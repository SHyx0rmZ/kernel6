OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH("i386:x86-64")
ENTRY(<%= config["kernel_entry"] %>)

SECTIONS
{
    . = <%= config["kernel_address"] %>;
    . = ALIGN(0x10);
    kernel_area_begin = .;
    .text :
    {
        . = ALIGN(0x10);
        *(.multiboot)
        . = ALIGN(0x10);
        *(.text)
    }
    .data :
    {
        . = ALIGN(0x10);
        start_ctors = .;
        *(.ctors)
        end_ctors = .;

        . = ALIGN(0x10);
        start_dtors = .;
        *(.dtors)
        end_dtors = .;

        . = ALIGN(0x10);
        *(.data)
        *(.rodata)
    }
    .bss :
    {
        . = ALIGN(0x10);
        *(.bss)
        *(.loader)
    }
    . = ALIGN(0x10);
    kernel_area_end = .;
}
