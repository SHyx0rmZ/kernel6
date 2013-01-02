#ifndef _LOADER_IDT_HPP_
#define _LOADER_IDT_HPP_

#include <cstdint>

struct IDTEntry
{
    std::uint16_t offset_low;
    std::uint16_t selector;
    std::uint16_t flags;
    std::uint16_t offset_high;
} __attribute__((packed));

struct IDTPointer
{
    std::uint16_t limit;
    std::uintptr_t base;
} __attribute__((packed));

extern "C" void isr_stub_0();
extern "C" void isr_stub_1();
extern "C" void isr_stub_2();
extern "C" void isr_stub_3();
extern "C" void isr_stub_4();
extern "C" void isr_stub_5();
extern "C" void isr_stub_6();
extern "C" void isr_stub_7();
extern "C" void isr_stub_8();
extern "C" void isr_stub_10();
extern "C" void isr_stub_11();
extern "C" void isr_stub_12();
extern "C" void isr_stub_13();
extern "C" void isr_stub_14();
extern "C" void isr_stub_16();
extern "C" void isr_stub_17();
extern "C" void isr_stub_18();
extern "C" void isr_stub_19();
extern "C" void isr_stub_32();
extern "C" void isr_stub_33();
extern "C" void isr_stub_34();
extern "C" void isr_stub_35();
extern "C" void isr_stub_36();
extern "C" void isr_stub_37();
extern "C" void isr_stub_38();
extern "C" void isr_stub_39();
extern "C" void isr_stub_40();
extern "C" void isr_stub_41();
extern "C" void isr_stub_42();
extern "C" void isr_stub_43();
extern "C" void isr_stub_44();
extern "C" void isr_stub_45();
extern "C" void isr_stub_46();
extern "C" void isr_stub_47();
extern "C" void isr_stub_null();

void idt_init();
void idt_load();

#endif
