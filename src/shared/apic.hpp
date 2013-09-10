#ifndef _SHARED_APIC_HPP_
#define _SHARED_APIC_HPP_

#include <cstdint>

namespace local_apic
{
    void Init(std::uint32_t base);
    void Write(std::uint32_t offset, std::uint32_t value);
    std::uint32_t Read(std::uint32_t offset);
}

#endif