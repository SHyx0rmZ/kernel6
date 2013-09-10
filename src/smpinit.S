.section .smpinit
.code16

    ljmp $0, $smp_start

.global smp_initializer_status
.global smp_initializer_stack
.global smp_initializer_context
.global smp_initializer_kernel

smp_initializer_status:
.byte 0
smp_initializer_stack:
.quad 0
smp_initializer_context:
.quad 0
smp_initializer_kernel:
.quad 0

.local smp_start
smp_start:

    /* disable interrupts, create stack and load GDT */
    cli
    movb $1, (smp_initializer_status)
    lgdt (smp_initializer_gdt_descriptor)

    mov %cr0, %eax
    or $1, %eax
    mov %eax, %cr0

    /* transfer control to known 32 bit segment */
    ljmp $8, $smp_switch_to_i386

.local smp_switch_to_i386
smp_switch_to_i386:

.code32

    /* load segment registers */
    movw $0, %dx
    movw $16, %cx
    movw %dx, %fs
    movw %dx, %gs
    movw %cx, %ds
    movw %cx, %es
    movw %cx, %ss

    movl $0, %esp

    movl %cr4, %eax
    orl $0x00a0, %eax
    movl %eax, %cr4

    movl (smp_initializer_context), %eax
    movl %eax, %cr3

    movl $0xc0000080, %ecx
    rdmsr
    orl $0x0100, %eax
    wrmsr

    movl %cr0, %eax
    orl $0x80000000, %eax
    movl %eax, %cr0

    ljmp $24, $smp_switch_to_amd64

.local smp_switch_to_amd64
smp_switch_to_amd64:

.code64

    lgdt (smp_initializer_gdt_descriptor)

    movq (smp_initializer_context), %rax
    movq %rax, %cr3

    movq (smp_initializer_stack), %rsp
    movq (smp_initializer_kernel), %rax
    call *%rax

.local smp_idle
smp_idle:

    cli
    hlt
    jmp smp_idle

.local smp_initializer_gdt
smp_initializer_gdt:
.int 0x00000000
.int 0x00000000
.int 0x0000ffff
.int 0x00cf9a00
.int 0x0000ffff
.int 0x00cf9200
.int 0x00000000
.int 0x00a09a00

.local smp_initializer_gdt_descriptor
smp_initializer_gdt_descriptor:
.short smp_initializer_gdt_descriptor - smp_initializer_gdt - 1
.quad smp_initializer_gdt