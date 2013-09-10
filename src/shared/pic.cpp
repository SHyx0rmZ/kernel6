#include "pic.hpp"
#include "io.hpp"

#include <cstdint>

namespace pic
{
    void Init()
    {
        /* tell the PIC it will get initialised */
        io::outb(0x20, 0x11);
        io::outb(0xA0, 0x11);

        /* map master to 0x20 and slave to 0x28 */
        io::outb(0x21, 0x20);
        io::outb(0xA1, 0x28);
        
        /* tell them about each other */
        io::outb(0x21, 0x04);
        io::outb(0xA1, 0x02);

        /* necessary for 80x86 */
        io::outb(0x21, 0x01);
        io::outb(0xA1, 0x01);

        /* mask nothing */
        io::outb(0x21, 0x00);
        io::outb(0xA1, 0x00);

        /* this doesn't really belong here, but I don't care for now */
        /* this pretty much generates an IRQ with 249.98 Hz */
        std::uint16_t timer_divisor = 4773;

        /* tell the PIT it will get initialised and send data */
        io::outb(0x43, 0x34);
        io::outb(0x40, (timer_divisor & 0xFF));
        io::outb(0x40, (timer_divisor >> 8));
    }
}