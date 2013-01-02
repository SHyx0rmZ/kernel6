#include "gdt.hpp"
#include "idt.hpp"

extern "C" void loader_bootstrap(void *)
{
    gdt_init();

    gdt_load();
}
