OUTPUT_FORMAT("elf64-x86-64")
OUTPUT_ARCH("i386:x86-64")
ENTRY(<%= config["kernel_entry"] %>)

SECTIONS
{
    kernel_area_begin = .;
    .text : AT(ADDR(.text) - <%= config["kernel_address"] %>)
    {
        . = ALIGN(0x10);
        *(.text)
    }
    .data : AT(ADDR(.data) - <%= config["kernel_address"] %>)
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
    .bss : AT(ADDR(.bss) - <%= config["kernel_address"] %>)
    {
        . = ALIGN(0x10);
        *(.bss)
        *(.loader)
        *(COMMON)
    }
    . = ALIGN(0x10);
    kernel_area_end = .;
}
