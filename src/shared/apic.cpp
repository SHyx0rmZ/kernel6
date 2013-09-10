#include "apic.hpp"

namespace local_apic
{
    static std::uint32_t base = 0;

    void Init(std::uint32_t base)
    {
        local_apic::base = base;

        Write(0xf0, (Read(0xf0) & 0x0200) | 0x015f);
    }

    void Write(std::uint32_t offset, std::uint32_t value)
    {
        std::uint32_t *pointer = reinterpret_cast<std::uint32_t *>(static_cast<std::uintptr_t>(base + offset));

        *pointer = value;
    }

    std::uint32_t Read(std::uint32_t offset)
    {
        std::uint32_t *pointer = reinterpret_cast<std::uint32_t *>(static_cast<std::uintptr_t>(base + offset));

        return *pointer;
    }
}