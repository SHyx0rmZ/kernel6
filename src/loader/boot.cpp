#include "gdt.hpp"
#include "idt.hpp"
#include "multiboot.hpp"
#include "console.hpp"
#include "smp.hpp"

extern "C" void loader_bootstrap(MultibootInfo *info)
{
    gdt_init();
    idt_init();

    gdt_load();
    idt_load();

    Console cout;

    cout << "Hello, world!";

    smp::Floating *floating = smp::FindFloating(0, 0x400);

    if (floating == nullptr)
    {
        floating = smp::FindFloating(0x9FC00, 0x400);
    }

    if (floating == nullptr)
    {
        floating = smp::FindFloating(0xE0000, 0x20000);
    }

    if (floating == nullptr || floating->config_table == 0 || floating->features[0] != 0)
    {
        cout << "SMP not supported!\n";
    }

    while (true);
}
