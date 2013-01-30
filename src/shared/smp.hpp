#ifndef _SHARED_SMP_HPP_
#define _SHARED_SMP_HPP_

#include <cstdint>
#include <cstddef>

namespace smp
{
    struct Floating
    {
        std::uint32_t signature;
        std::uint32_t config_table;
        std::uint8_t length;
        std::uint8_t version;
        std::uint8_t checksum;
        std::uint8_t features[5];
    } __attribute__((packed));

    namespace config
    {
        struct Table
        {
            std::uint32_t signature;
            std::uint16_t length;
            std::uint8_t version;
            std::uint8_t checksum;
            std::uint8_t oem_id[8];
            std::uint8_t product_id[12];
            std::uint32_t oem_table;
            std::uint16_t oem_table_size;
            std::uint16_t entry_count;
            std::uint32_t local_apic;
            std::uint16_t ext_table_length;
            std::uint8_t ext_table_checksum;
            std::uint8_t reserved;
        } __attribute__((packed));

        struct Processor
        {
            std::uint8_t entry_type;
            std::uint8_t local_apic_id;
            std::uint8_t local_apic_version;
            std::uint8_t flags;
            std::uint8_t signature[4];
            std::uint32_t feature_flags;
            std::uint32_t feature_reserved[2];
        } __attribute__((packed));

        struct Bus
        {
            std::uint8_t entry_type;
            std::uint8_t id;
            std::uint8_t type[6];
        } __attribute__((packed));

        struct IOAPIC
        {
            std::uint8_t entry_type;
            std::uint8_t id;
            std::uint8_t version;
            std::uint8_t flags;
            std::uint8_t address;
        } __attribute__((packed));

        struct IOAssignment
        {
            std::uint8_t entry_type;
            std::uint8_t type;
            std::uint16_t flag;
            std::uint8_t src_id;
            std::uint8_t src_irq;
            std::uint8_t dest_id;
            std::uint8_t dest_int;
        } __attribute__((packed));

        struct LocalAssignment
        {
            std::uint8_t entry_type;
            std::uint8_t type;
            std::uint16_t flag;
            std::uint8_t src_id;
            std::uint8_t src_irq;
            std::uint8_t dest_id;
            std::uint8_t dest_int;
        } __attribute__((packed));

        struct Entry
        {
            std::uint8_t entry_type;
            std::uint8_t reserved[7];
        } __attribute__((packed));
    }

    struct StartupData
    {
        std::uint8_t jump[5];
        std::uint8_t status;
        std::uint64_t stack;
        std::uint64_t context;
        std::uint64_t kernel;
    } __attribute__((packed));

    Floating *FindFloating(std::uintptr_t start, std::size_t size);
}

#define SMP_FLAG_ACTIVE 1
#define SMP_FLAG_BOOT   2

#endif
