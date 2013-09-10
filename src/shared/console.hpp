#ifndef _SHARED_CONSOLE_HPP_
#define _SHARED_CONSOLE_HPP_

#include <cstdint>
//#include <string>

#define endl "\r\n"

struct Console
{
    Console();
    ~Console();

    Console &operator<<(const char *const str);
    //Console &operator<<(const std::string &str);
    Console &operator<<(std::uint8_t num);
    Console &operator<<(std::uint16_t num);
    Console &operator<<(std::uint32_t num);
    Console &operator<<(std::uint64_t num);
    Console &operator<<(std::int8_t num);
    Console &operator<<(std::int16_t num);
    Console &operator<<(std::int32_t num);
    Console &operator<<(std::int64_t num);
    Console &operator<<(const void *const ptr);
    Console &operator<<(const char c);

    private:

    std::uint16_t *video;
};

#endif
