OUTPUT_FORMAT("binary")
OUTPUT_ARCH("i386:x86-64")

SECTIONS
{
    .smpinit <%= config["smp_address"] %> : AT(<%= config["smp_address"] %>)
    {
        *(.smpinit)
        *(.text)
        *(.data)
    }
}