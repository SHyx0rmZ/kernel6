.section .multiboot

#define LOADER_MB_MAGIC 0x1badb002
#define LOADER_MB_FLAGS 0x0003
#define LOADER_MB_CHECKSUM -(LOADER_MB_MAGIC + LOADER_MB_FLAGS)

#define LOADER_MB2_MAGIC 0xe85250d6
#define LOADER_MB2_ARCHITECTURE 0
#define LOADER_MB2_CHECKSUM -(LOADER_MB2_MAGIC + LOADER_MB2_ARCHITECTURE + loader_multiboot2_header_end - loader_multiboot2_header)

.align 4

.int LOADER_MB_MAGIC
.int LOADER_MB_FLAGS
.int LOADER_MB_CHECKSUM

/* unused */
.int 0
.int 0
.int 0
.int 0
.int 0

/* graphics mode 1, 80x25 chars, automatically select depth */
.int 1
.int 80
.int 25
.int 0


.align 8

loader_multiboot2_header:

.int LOADER_MB2_MAGIC
.int LOADER_MB2_ARCHITECTURE
.int loader_multiboot2_header_end - loader_multiboot2_header
.int LOADER_MB2_CHECKSUM

loader_multiboot2_information_request:

.short 1
.short 0
.int loader_multiboot2_information_request_end - loader_multiboot2_information_request
.int 3
.int 6
.int 8
.int 0

loader_multiboot2_information_request_end:

.short 1
.short 0
.int 16
.int 7
.int 0

.short 3
.short 0
.int 12
.int <%= config["loader_entry"] %>

.short 5
.short 0
.int 20
.int 80
.int 25
.int 0

.short 0
.short 0
.int 8

loader_multiboot2_header_end:

/* provide extra space for stack */
.section .loader, "aw", @nobits

.align 0x1000
.space <%= config["loader_stack_size"] %>
.set LOADER_STACK_ADDRESS, .

.section .bootstrap

.global loader_entry
loader_entry:
    cli
    movl $LOADER_STACK_ADDRESS, %esp
    lgdt (loader_gdt_descriptor)

    ljmp $8, $loader_switch_to_protected_mode

.local loader_switch_to_protected_mode
loader_switch_to_protected_mode:
    movw $0, %dx
    movw $16, %cx
    movw %dx, %fs
    movw %dx, %gs
    movw %cx, %ds
    movw %cx, %es
    movw %cx, %ss

    call loader_bootstrap

.local loader_idle
loader_idle:
    cli
    hlt
    jmp loader_idle

.align 1

.local loader_gdt
loader_gdt:

.int 0x00000000
.int 0x00000000
.int 0x0000ffff
.int 0x00cf9a00
.int 0x0000ffff
.int 0x00cf9200

.local loader_gdt_descriptor
loader_gdt_descriptor:

.short loader_gdt_descriptor - loader_gdt - 1
.int loader_gdt
