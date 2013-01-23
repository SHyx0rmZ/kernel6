#include "gdt.hpp"

// FIXME
// std::array<GDTEntry, 7> gdt;
GDTEntry gdt[5];

void gdt_set_entry(std::uint8_t index, std::uint32_t base, std::uint32_t limit, std::uint8_t access, std::uint8_t flags)
{
    gdt[index].limit_low = limit & 0xFFFF;
    gdt[index].flags_and_limit_high = ((flags << 4) & 0xF0) | ((limit >> 16) & 0xF);
    gdt[index].base_low = base & 0xFFFF;
    gdt[index].base_middle = (base >> 16) & 0xFF;
    gdt[index].base_high = (base >> 24) & 0xFF;
    gdt[index].access = access;
}

void gdt_init()
{
    // null segment
    gdt_set_entry(0, 0, 0, 0, 0);
    // 32 bit code, ring 0
    gdt_set_entry(1, GDT_BASE_NULL, GDT_LIMIT_FULL, GDT_PRESENT | GDT_RING_0 | GDT_SEGMENT | GDT_EXECUTABLE | GDT_READABLE, GDT_GRANULAR | GDT_PROTECTED_MODE);
    // 32 bit data, ring 0
    gdt_set_entry(2, GDT_BASE_NULL, GDT_LIMIT_FULL, GDT_PRESENT | GDT_RING_0 | GDT_SEGMENT | GDT_WRITABLE | GDT_DIRECTION_UP, GDT_GRANULAR | GDT_PROTECTED_MODE);
    // 64 bit code, ring 3
    gdt_set_entry(3, GDT_BASE_NULL, GDT_LIMIT_FULL, GDT_PRESENT | GDT_RING_3 | GDT_SEGMENT | GDT_EXECUTABLE | GDT_READABLE, GDT_GRANULAR | GDT_LONG_MODE);
    // 32 bit data, ring 3
    gdt_set_entry(4, GDT_BASE_NULL, GDT_LIMIT_FULL, GDT_PRESENT | GDT_RING_3 | GDT_SEGMENT | GDT_WRITABLE | GDT_DIRECTION_UP, GDT_GRANULAR | GDT_PROTECTED_MODE);
    // 64 bit code, ring 0
    gdt_set_entry(5, GDT_BASE_NULL, GDT_LIMIT_FULL, GDT_PRESENT | GDT_RING_0 | GDT_SEGMENT | GDT_EXECUTABLE | GDT_READABLE, GDT_GRANULAR | GDT_LONG_MODE);
}

void gdt_load()
{
    GDTPointer gdtp;

    // FIXME
    // gdtp.limit = gdt.size() * sizeof(GDTEntry) - 1;
    gdtp.limit = 5 * sizeof(GDTEntry) - 1;
    gdtp.base = reinterpret_cast<std::uintptr_t>(gdt);

    struct
    {
        std::uint32_t offset;
        std::uint16_t segment;
    } jump_address;

    jump_address.segment = 0x08;

    asm volatile (
        "lgdt %0 \n"
        "mov %1, %%ds \n"
        "mov %1, %%es \n"
        "mov %1, %%ss \n"
        "mov %2, %%fs \n"
        "mov %2, %%gs \n"
        "movl $gdt_jump, %3 \n"
        "ljmpl *%3 \n"
        "gdt_jump: \n"
        :: "m"(gdtp), "r"(0x10), "r"(0x00), "m"(jump_address) : "memory"
    );
}
