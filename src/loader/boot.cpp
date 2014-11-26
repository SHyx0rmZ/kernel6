#include "gdt.hpp"
#include "idt.hpp"
#include "multiboot.hpp"
#include "console.hpp"
#include "smp.hpp"
#include "memory.hpp"
#include "pic.hpp"

#include <new>

extern "C" void loader_bootstrap(MultibootInfo *info)
{
    gdt_init();
    idt_init();

    gdt_load();
    idt_load();

    pic::Init();

    Console cout;

    cout << "Hello, world!" << endl;

    memory_init(info);

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

    cout << "SMP : " << floating << endl;

    char *memory1 = new char[2041];
    float *memory2 = new float[17324];

    cout << "MEM1: " << (void*)memory1 << endl;
    cout << "MEM2: " << memory2 << endl;

    delete[] memory1;
    delete[] memory2;

    void *memory3 = memory_alloc(371);

    cout << "MEM3: " << memory3 << endl;

    while (true) {
        char *tmp = new char[1024];
        //cout << (void*)tmp << endl;
        delete[] tmp;
    }
}
