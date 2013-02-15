#include "smp.hpp"

namespace smp
{
    Floating *FindFloating(std::uintptr_t start, std::size_t size)
    {
        Floating *floating = reinterpret_cast<Floating *>(start);

        while (reinterpret_cast<std::uintptr_t>(floating) < start + size)
        {
            if (floating->signature == 0x5f504d5f)
            {
                std::uint8_t checksum = 0;
                std::uint8_t *pointer = reinterpret_cast<std::uint8_t *>(floating);

                for (std::size_t i = 0; i < sizeof(Floating); ++i)
                {
                    checksum += pointer[i];
                }

                if (checksum == 0)
                {
                    return floating;
                }
            }

            floating++;
        }

        return nullptr;
    }
}
