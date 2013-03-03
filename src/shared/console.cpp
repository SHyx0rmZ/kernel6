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

Console &Console::operator<<(std::uint8_t num)
{
    return *this << static_cast<std::uint64_t>(num);
}

Console &Console::operator<<(std::uint16_t num)
{
    return *this << static_cast<std::uint64_t>(num);
}

Console &Console::operator<<(std::uint32_t num)
{
    return *this << static_cast<std::uint64_t>(num);
}

Console &Console::operator<<(std::uint64_t num)
{
    char string[21];

    string[20] = '\0';

    char *p = &string[20];

    if (num)
        for (char rest = num % 10; num; num /= 10, rest = num % 10)
        {
            *(--p) = rest + '0';
        }
    else
        *(--p) = '0';

    return *this << p;
}

Console &Console::operator<<(const void *const ptr)
{
    char c[3] = { '0', 'x', '\0' };

    *this << c;

    for (int i = sizeof(void *); i; --i)
    {
        c[0] = (reinterpret_cast<const char *const>(&ptr)[i - 1] >> 4) & 15;
        c[1] = reinterpret_cast<const char *const>(&ptr)[i - 1] & 15;

        c[0] += c[0] > 9 ? '7' : '0';
        c[1] += c[1] > 9 ? '7' : '0';

        *this << c;
    }

    return *this;
}

Console &Console::operator<<(const char c)
{
    char string[] = { c, '\0' };

    return *this << string;
}
