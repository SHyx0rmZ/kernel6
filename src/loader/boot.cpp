#include "gdt.hpp"
#include "idt.hpp"

extern "C" void loader_bootstrap(void *)
{
    gdt_init();
    idt_init();

    gdt_load(0x08, nullptr);
    idt_load();
}
