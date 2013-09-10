#include "idt.hpp"

// FIXME
// std::array<IDTEntry, 256> idt;
IDTEntry idt[256];

void idt_set_entry(std::uint8_t index, std::uintptr_t offset, std::uint16_t selector, std::uint16_t flags)
{
    idt[index].offset_low = offset & 0xFFFF;
    idt[index].offset_high = (offset >> 16) & 0xFFFF;
    idt[index].selector = selector;
    idt[index].flags = flags;
}

void idt_init()
{
    for (std::uint16_t i = 0; i < 256; i++)
    {
        idt_set_entry(i, reinterpret_cast<std::uintptr_t>(isr_stub_null), 0x08, 0x8E01);
    }

    idt_set_entry(0, reinterpret_cast<std::uintptr_t>(&isr_stub_0), 0x08, 0x8E01);
    idt_set_entry(1, reinterpret_cast<std::uintptr_t>(&isr_stub_1), 0x08, 0x8E01);
    idt_set_entry(2, reinterpret_cast<std::uintptr_t>(&isr_stub_2), 0x08, 0x8E01);
    idt_set_entry(3, reinterpret_cast<std::uintptr_t>(&isr_stub_3), 0x08, 0x8E01);
    idt_set_entry(4, reinterpret_cast<std::uintptr_t>(&isr_stub_4), 0x08, 0x8E01);
    idt_set_entry(5, reinterpret_cast<std::uintptr_t>(&isr_stub_5), 0x08, 0x8E01);
    idt_set_entry(6, reinterpret_cast<std::uintptr_t>(&isr_stub_6), 0x08, 0x8E01);
    idt_set_entry(7, reinterpret_cast<std::uintptr_t>(isr_stub_7), 0x08, 0x8E01);
    idt_set_entry(8, reinterpret_cast<std::uintptr_t>(isr_stub_8), 0x08, 0x8E01);
    idt_set_entry(10, reinterpret_cast<std::uintptr_t>(isr_stub_10), 0x08, 0x8E01);
    idt_set_entry(11, reinterpret_cast<std::uintptr_t>(isr_stub_11), 0x08, 0x8E01);
    idt_set_entry(12, reinterpret_cast<std::uintptr_t>(isr_stub_12), 0x08, 0x8E01);
    idt_set_entry(13, reinterpret_cast<std::uintptr_t>(isr_stub_13), 0x08, 0x8E01);
    idt_set_entry(14, reinterpret_cast<std::uintptr_t>(isr_stub_14), 0x08, 0x8E01);
    idt_set_entry(16, reinterpret_cast<std::uintptr_t>(isr_stub_16), 0x08, 0x8E01);
    idt_set_entry(17, reinterpret_cast<std::uintptr_t>(isr_stub_17), 0x08, 0x8E01);
    idt_set_entry(18, reinterpret_cast<std::uintptr_t>(isr_stub_18), 0x08, 0x8E01);
    idt_set_entry(19, reinterpret_cast<std::uintptr_t>(isr_stub_19), 0x08, 0x8E01);

    idt_set_entry(32, reinterpret_cast<std::uintptr_t>(isr_stub_32), 0x08, 0x8E01);
    idt_set_entry(33, reinterpret_cast<std::uintptr_t>(isr_stub_33), 0x08, 0x8E01);
    idt_set_entry(34, reinterpret_cast<std::uintptr_t>(isr_stub_34), 0x08, 0x8E01);
    idt_set_entry(35, reinterpret_cast<std::uintptr_t>(isr_stub_35), 0x08, 0x8E01);
    idt_set_entry(36, reinterpret_cast<std::uintptr_t>(isr_stub_36), 0x08, 0x8E01);
    idt_set_entry(37, reinterpret_cast<std::uintptr_t>(isr_stub_37), 0x08, 0x8E01);
    idt_set_entry(38, reinterpret_cast<std::uintptr_t>(isr_stub_38), 0x08, 0x8E01);
    idt_set_entry(39, reinterpret_cast<std::uintptr_t>(isr_stub_39), 0x08, 0x8E01);
    idt_set_entry(40, reinterpret_cast<std::uintptr_t>(isr_stub_40), 0x08, 0x8E01);
    idt_set_entry(41, reinterpret_cast<std::uintptr_t>(isr_stub_41), 0x08, 0x8E01);
    idt_set_entry(42, reinterpret_cast<std::uintptr_t>(isr_stub_42), 0x08, 0x8E01);
    idt_set_entry(43, reinterpret_cast<std::uintptr_t>(isr_stub_43), 0x08, 0x8E01);
    idt_set_entry(44, reinterpret_cast<std::uintptr_t>(isr_stub_44), 0x08, 0x8E01);
    idt_set_entry(45, reinterpret_cast<std::uintptr_t>(isr_stub_45), 0x08, 0x8E01);
    idt_set_entry(46, reinterpret_cast<std::uintptr_t>(isr_stub_46), 0x08, 0x8E01);
    idt_set_entry(47, reinterpret_cast<std::uintptr_t>(isr_stub_47), 0x08, 0x8E01);

}

void idt_load()
{
    IDTPointer idtp;

    // FIXME
    // idtp.limit = idt.size() * sizeof(IDTEntry) - 1;
    idtp.limit = 256 * sizeof(IDTEntry) - 1;
    idtp.base = reinterpret_cast<std::uintptr_t>(idt);

    asm volatile (
        "lidt %0 \n"
        :: "m"(idtp)
    );
}
