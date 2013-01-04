#ifndef _LOADER_GDT_HPP_
#define _LOADER_GDT_HPP_

#include <cstdint>

void gdt_set_entry(std::uint8_t index, std::uint32_t base, std::uint32_t limit, std::uint8_t access, std::uint8_t flags);

struct GDTEntry
{
    std::uint16_t limit_low;
    std::uint16_t base_low;
    std::uint8_t base_middle;
    std::uint8_t access;
    std::uint8_t flags_and_limit_high;
    std::uint8_t base_high;
} __attribute__((packed));

struct GDTPointer
{
    std::uint16_t limit;
    std::uintptr_t base;
} __attribute__((packed));

#define GDT_BASE_NULL       0
#define GDT_LIMIT_FULL      0xffffffff

#define GDT_PRESENT         0x80
#define GDT_RING_3          0x60
#define GDT_RING_2          0x40
#define GDT_RING_1          0x20
#define GDT_RING_0          0x00
#define GDT_TSS             0x09
#define GDT_SEGMENT         0x10
#define GDT_EXECUTABLE      0x08
#define GDT_DIRECTION_DOWN  0x04
#define GDT_DIRECTION_UP    0x00
#define GDT_CONFORMING      0x04
#define GDT_READABLE        0x02
#define GDT_WRITABLE        0x02
#define GDT_ACCESSED        0x01
#define GDT_NULL            0x00

#define GDT_GRANULAR        0x08
#define GDT_PROTECTED_MODE  0x04
#define GDT_REAL_MODE       0x00
#define GDT_LONG_MODE       0x02
#define GDT_AVAILABLE       0x01

void gdt_init();
void gdt_load();

#endif
