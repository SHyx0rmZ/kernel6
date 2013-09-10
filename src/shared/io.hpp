#ifndef _SHARED_IO_HPP_
#define _SHARED_IO_HPP_

#include <cstdint>

namespace io
{
    std::uint8_t inb(std::uint16_t port);
    void outb(std::uint16_t port, std::uint8_t value);
}

#endif