#include "io.hpp"

namespace io
{
    std::uint8_t inb(std::uint16_t port)
    {
        std::uint8_t value;

        asm volatile(
            "inb %%dx, %%al"
            : "=a"(value)
            : "d"(port)
        );

        return value;
    }

    void outb(std::uint16_t port, std::uint8_t value)
    {
        asm volatile(
            "outb %%al, %%dx"
            : : "a"(value), "d"(port)
        );
    }
}