#include <cstdint>
//#include <string>
#include <cstring>

#include "console.hpp"
#include "io.hpp"

#define CONSOLE_USE_SERIAL 1

#define CONSOLE_AREA 0xB8000
#define CONSOLE_START_LINE 2
#define CONSOLE_START CONSOLE_AREA + CONSOLE_START_LINE * 160
#define CONSOLE_COLOR 0x0700

#if CONSOLE_USE_SERIAL == 1
#define SERIAL_DIVISOR_LATCH_REGISTER_LSB   0x3F8
#define SERIAL_TRANSMITTER_HOLDING_REGISTER 0x3F8
#define SERIAL_RECEIVER_BUFFER_REGISTER     0x3F8
#define SERIAL_DIVISOR_LATCH_REGISTER_MSB   0x3F9
#define SERIAL_INTERRUPT_ENABLE_REGISTER    0x3F9
#define SERIAL_FIFO_CONTROL_REGISTER        0x3FA
#define SERIAL_LINE_CONTROL_REGISTER        0x3FB
#define SERIAL_MODEM_CONTROL_REGISTER       0x3FC
#define SERIAL_LINE_STATUS_REGISTER         0x3FD

#define SERIAL_WORD_LENGTH_SELECTOR_8BIT        0x03
#define SERIAL_TRANSMITTER_HOLDING_EMPTY_BIT    0x20
#define SERIAL_DIVISOR_LATCH_ACCESS_BIT         0x80
#endif

Console::Console()
{
#if CONSOLE_USE_SERIAL == 1
    io::outb(SERIAL_LINE_CONTROL_REGISTER, SERIAL_DIVISOR_LATCH_ACCESS_BIT | SERIAL_WORD_LENGTH_SELECTOR_8BIT);
    io::outb(SERIAL_DIVISOR_LATCH_REGISTER_LSB, 0x0C);
    io::outb(SERIAL_DIVISOR_LATCH_REGISTER_MSB, 0x00);
    io::outb(SERIAL_LINE_CONTROL_REGISTER, SERIAL_WORD_LENGTH_SELECTOR_8BIT);
    io::outb(SERIAL_INTERRUPT_ENABLE_REGISTER, 0);
    io::outb(SERIAL_FIFO_CONTROL_REGISTER, 0);
    io::outb(SERIAL_MODEM_CONTROL_REGISTER, 0);
#endif

    std::memset(reinterpret_cast<void *>(CONSOLE_AREA), 0, 25 * 160);

    this->video = reinterpret_cast<std::uint16_t *>(CONSOLE_AREA);

    *this << "kernel6 >>" << endl;

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
#if CONSOLE_USE_SERIAL == 1
        io::outb(SERIAL_TRANSMITTER_HOLDING_REGISTER, *character);

        while ((io::inb(SERIAL_LINE_STATUS_REGISTER) & SERIAL_TRANSMITTER_HOLDING_EMPTY_BIT) != SERIAL_TRANSMITTER_HOLDING_EMPTY_BIT);
#endif

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

Console &Console::operator<<(std::int8_t num)
{
    return *this << static_cast<std::int64_t>(num);
}

Console &Console::operator<<(std::int16_t num)
{
    return *this << static_cast<std::int64_t>(num);
}

Console &Console::operator<<(std::int32_t num)
{
    return *this << static_cast<std::int64_t>(num);
};

Console &Console::operator<<(std::int64_t num)
{
    if (num < 0)
        *this << "-";

    return *this << static_cast<std::uint64_t>(num < 0 ? -num : num);
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
