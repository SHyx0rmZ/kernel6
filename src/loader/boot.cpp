#include "gdt.hpp"
#include "idt.hpp"
#include "multiboot.hpp"
#include "console.hpp"
#include "smp.hpp"
#include "memory.hpp"
#include "pic.hpp"

extern "C" void loader_bootstrap(MultibootInfo *info)
{
    gdt_init();
    idt_init();

    gdt_load();
    idt_load();

    pic::Init();

    Console cout;

    cout << "Hello, world!" << endl;

    std::size_t free, used;

    memory_init(info);
    memory_status(&free, &used);

    cout << "Initialized memory (" << used/1024 << " KiB / " << (used+free)/1024 << " KiB used)" << endl;

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
