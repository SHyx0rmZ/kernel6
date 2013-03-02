#include <cstdint>
#include <cstddef>

#include "memory.hpp"

struct MemoryNode
{
    MemoryNode *next;
    std::uint64_t address;
    std::uint64_t size;
} __attribute__((packed));

MemoryNode *list_free = nullptr;
MemoryNode *list_used = nullptr;

void memory_init(MultibootInfo *info)
{
    MultibootMemory *memory_begin = reinterpret_cast<MultibootMemory *>(info->mmap_addr);
    MultibootMemory *memory_end = reinterpret_cast<MultibootMemory *>(static_cast<std::uintptr_t>(info->mmap_addr) + info->mmap_length);

    unsigned int num_free = 0, num_used = 1;

    for (MultibootMemory *memory = memory_begin; memory < memory_end; memory = reinterpret_cast<MultibootMemory *>(reinterpret_cast<std::uintptr_t>(memory) + memory->size + 4))
    {
        if (memory->type == 1)
            num_free++;
        else
            num_used++;
    }

    std::uintptr_t max = 0;
    const std::size_t struct_size = (num_free + num_used) * sizeof(MemoryNode);

    for (MultibootMemory *memory = memory_begin; memory < memory_end; memory = reinterpret_cast<MultibootMemory *>(reinterpret_cast<std::uintptr_t>(memory) + memory->size + 4))
    {
        // find highest memory block below 4 GiB that has enough space for the initial memory nodes
        if (memory->type == 1 && memory->address > max && memory->length > struct_size && memory->address + struct_size < -1U)
            max = memory->address;
    }

    MemoryNode *node = reinterpret_cast<MemoryNode *>(max);

    list_free = node;

    for (MultibootMemory *memory = memory_begin; memory < memory_end; memory = reinterpret_cast<MultibootMemory *>(reinterpret_cast<std::uintptr_t>(memory) + memory->size + 4))
    {
        if (memory->type == 1)
        {
            if (memory->address == max)
            {
                memory->address += struct_size;
                memory->length -= struct_size;
            }

            node->address = memory->address;
            node->size = memory->length;
            node->next = node + 1;
            node = node->next;
        }
    }

    (node - 1)->next = nullptr;
    list_used = node;

    node->address = max;
    node->size = struct_size;
    node->next = node + 1;
    node = node->next;

    for (MultibootMemory *memory = memory_begin; memory < memory_end; memory = reinterpret_cast<MultibootMemory *>(reinterpret_cast<std::uintptr_t>(memory) + memory->size + 4))
    {
        if (memory->type != 1)
        {
            node->address = memory->address;
            node->size = memory->length;
            node->next = node + 1;
            node = node->next;
        }
    }

    (node - 1)->next = nullptr;
}
