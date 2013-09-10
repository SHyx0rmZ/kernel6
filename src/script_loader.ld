OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH("i386")
ENTRY(<%= config["loader_entry"] %>)

SECTIONS
{
    . = <%= config["loader_address"] %>;
    kernel_area_begin = .;
    .text :
    {
        . = ALIGN(0x10);
        *(.multiboot)
        . = ALIGN(0x10);
        *(.bootstrap)
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

        . = ALIGN(0x10);
        payload_begin = .;
        *(.payload)
        payload_end = .;
    }
    .bss :
    {
        . = ALIGN(0x10);
        *(.bss)
        *(.loader)
        *(COMMON)
    }
    kernel_area_end = .;
}
