#include <cstdint>
//#include <string>
#include <cstring>

#include "console.hpp"

#define CONSOLE_AREA 0xB8000
#define CONSOLE_START_LINE 2
#define CONSOLE_START CONSOLE_AREA + CONSOLE_START_LINE * 160
#define CONSOLE_COLOR 0x0700


Console::Console()
{
    this->video = reinterpret_cast<std::uint16_t *>(CONSOLE_START);
}

Console::~Console()
{
}

Console &Console::operator<<(const char *const str)
{
    const char *character = str;

    while (*character)
    {
        switch (*character)
        {
            case '\n':
                video = reinterpret_cast<std::uint16_t *>(reinterpret_cast<std::uintptr_t>(video) + 160);
            case '\r':
                video = reinterpret_cast<std::uint16_t *>(reinterpret_cast<std::uintptr_t>(video) - ((reinterpret_cast<std::uintptr_t>(video) - CONSOLE_AREA) % 160));
                break;
            default:
                while (reinterpret_cast<std::uintptr_t>(video) >= (CONSOLE_AREA + (160 * 25)))
                {
                    std::memmove(reinterpret_cast<void *>(CONSOLE_START), reinterpret_cast<void *>(CONSOLE_START + 160), (24 - CONSOLE_START_LINE) * 160);
                    std::memset(reinterpret_cast<void *>(CONSOLE_AREA + 24 * 160), 0, 160);

                    video = reinterpret_cast<std::uint16_t *>(reinterpret_cast<std::uintptr_t>(video) - 160);
                }

                *(video++) = CONSOLE_COLOR | *character;

                break;
        }

        character++;
    }

    return *this;
}

//Console &operator<<(const std::string &str)

Console &Console::operator<<(std::uint64_t)
{
    return *this;
}

Console &Console::operator<<(const void *const)
{
    return *this;
}
