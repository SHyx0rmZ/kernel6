#include "cpustate.hpp"
#include "console.hpp"
#include "io.hpp"

extern "C" cpustate *isr_handler(cpustate *state)
{
    if (state->vector < 0x20)
    {
        Console cout;

        cout << "Exception encountered, stopping execution!" << endl << endl;

        cout << "EIP " << (void*)state->eip << endl;
        cout << "VEC " << (void*)state->vector << endl;
        cout << "ERR " << (void*)state->errorcode << endl;

        while (true)
            asm volatile("hlt");
    }
    else if (state->vector < 0x30)
    {
        if (state->vector >= 0x28)
        {
            io::outb(0xA0, 0x20);
        }

        io::outb(0x20, 0x20);
    }

    return state;
}
