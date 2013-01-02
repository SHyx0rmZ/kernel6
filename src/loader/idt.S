.macro isr_stub nr
    .global isr_stub_\nr
    isr_stub_\nr:
    pushl $0
    pushl $\nr
    jmp isr_stub_common
.endm

.macro isr_stub_error nr
    .global isr_stub_\nr
    isr_stub_\nr:
    pushl $\nr
    jmp isr_stub_common
.endm

.macro isr_named_stub name,nr
    .global isr_stub_\name
    isr_stub_\name:
    pushl $0
    pushl $\nr
    jmp isr_stub_common
.endm

isr_stub 0
isr_stub 1
isr_stub 2
isr_stub 3
isr_stub 4
isr_stub 5
isr_stub 6
isr_stub 7
isr_stub_error 8
isr_stub_error 10
isr_stub_error 11
isr_stub_error 12
isr_stub_error 13
isr_stub_error 14
isr_stub 16
isr_stub_error 17
isr_stub 18
isr_stub 19

isr_stub 32
isr_stub 33
isr_stub 34
isr_stub 35
isr_stub 36
isr_stub 37
isr_stub 38
isr_stub 39
isr_stub 40
isr_stub 41
isr_stub 42
isr_stub 43
isr_stub 44
isr_stub 45
isr_stub 46
isr_stub 47

isr_named_stub null, 0xdead

.global isr_stub_common
isr_stub_common:
    pushl %eax
    pushl %ecx
    pushl %edx
    pushl %ebx
    pushl %esi
    pushl %edi
    pushl %ebp

    pushl $0
    pushl $0
    pushl $0
    pushl $0

    pushl %esp

    movw %ds, 16(%esp)
    movw %es, 12(%esp)
    movw %fs, 8(%esp)
    movw %gs, 4(%esp)

    call isr_handler

    movw 4(%eax), %gs
    movw 8(%eax), %fs
    movw 12(%eax), %es
    movw 16(%eax), %ds

    addl $16, %eax
    movl %eax, %esp

    popl %ebp
    popl %edi
    popl %esi
    popl %ebx
    popl %edx
    popl %ecx
    popl %eax

    addl $8, %esp
    iret
