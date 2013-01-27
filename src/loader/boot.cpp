#include "gdt.hpp"
#include "idt.hpp"
#include "console.hpp"

extern "C" void loader_bootstrap(void *)
{
    gdt_init();
    idt_init();

    gdt_load();
    idt_load();

    Console cout;

    cout << "Hello, world!";

    while (true);
}
