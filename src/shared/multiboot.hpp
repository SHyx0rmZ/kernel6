#ifndef _MULTIBOOT_HPP_
#define _MULTIBOOT_HPP_

#include <cstdint>

struct MultibootMemory
{
    std::uint32_t size;
    std::uint64_t address;
    std::uint64_t length;
    std::uint32_t type;
} __attribute__((packed));

struct MultibootModule
{
    std::uint32_t mod_start;
    std::uint32_t mod_end;
    std::uint32_t name;
    std::uint32_t reserved;
} __attribute__((packed));

struct MultibootInfo
{
    std::uint32_t flags;
    std::uint32_t mem_lower;
    std::uint32_t mem_upper;
    std::uint32_t boot_device;
    char *cmdline;
    std::uint32_t mods_count;
    MultibootModule *mods_addr;
    std::uint32_t elfshdr_num;
    std::uint32_t elfshdr_size;
    std::uint32_t elfshdr_addr;
    std::uint32_t elfshdr_shndx;
    std::uint32_t mmap_length;
    MultibootMemory *mmap_addr;
    std::uint32_t drives_length;
    std::uint32_t drives_addr;
    std::uint32_t config_table;
    char *boot_loader_name;
    std::uint32_t apm_table;
    std::uint32_t vbe_control_info;
    std::uint32_t vbe_mode_info;
    std::uint16_t vbe_mode;
    std::uint16_t vbe_interface_seg;
    std::uint16_t vbe_interface_off;
    std::uint16_t vbe_interface_len;
    std::uint32_t size;
} __attribute__((packed));

#endif
