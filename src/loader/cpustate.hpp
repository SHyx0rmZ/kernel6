#ifndef _LOADER_CPUSTATE_HPP_
#define _LOADER_CPUSTATE_HPP_

#include <cstdint>

struct cpustate
{
    std::uint32_t gs;
    std::uint32_t fs;
    std::uint32_t es;
    std::uint32_t ds;
    std::uint32_t ebp;
    std::uint32_t edi;
    std::uint32_t esi;
    std::uint32_t ebx;
    std::uint32_t edx;
    std::uint32_t ecx;
    std::uint32_t eax;
    std::uint32_t vector;
    std::uint32_t errorcode;
    std::uint32_t eip;
    std::uint32_t cs;
    std::uint32_t eflags;
    std::uint32_t esp;
    std::uint32_t ss;
} __attribute__((packed));

#endif
