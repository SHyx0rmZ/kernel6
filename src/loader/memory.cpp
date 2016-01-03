#include <cstdint>

#include "memory.hpp"

struct MemoryNode
{
    MemoryNode *next;
    std::uint64_t address;
    std::uint64_t size;
} __attribute__((packed));

MemoryNode *list_free = nullptr;
MemoryNode *list_used = nullptr;

extern "C" void kernel_area_begin(void);
extern "C" void kernel_area_end(void);

void memory_init(MultibootInfo *info)
{
    MultibootMemory *memory_begin = reinterpret_cast<MultibootMemory *>(info->mmap_addr);
    MultibootMemory *memory_end = reinterpret_cast<MultibootMemory *>(static_cast<std::uintptr_t>(info->mmap_addr) + info->mmap_length);

    // one additional used block for memory management
    unsigned int num_free = 0, num_used = 1;

    for (MultibootMemory *memory = memory_begin; memory < memory_end; memory = reinterpret_cast<MultibootMemory *>(reinterpret_cast<std::uintptr_t>(memory) + memory->size + 4))
    {
        if (memory->type == MEMORY_TYPE_FREE)
            num_free++;
        else
            num_used++;
    }

    std::uintptr_t max = 0;
    const std::size_t struct_size = (num_free + num_used) * sizeof(MemoryNode);

    for (MultibootMemory *memory = memory_begin; memory < memory_end; memory = reinterpret_cast<MultibootMemory *>(reinterpret_cast<std::uintptr_t>(memory) + memory->size + 4))
    {
        // find highest memory block below 4 GiB that has enough space for the initial memory nodes
        if (memory->type == MEMORY_TYPE_FREE && memory->address > max && memory->length > struct_size && memory->address + struct_size < -1U)
            max = memory->address;
    }

    MemoryNode *node = reinterpret_cast<MemoryNode *>(max);

    list_free = node;

    for (MultibootMemory *memory = memory_begin; memory < memory_end; memory = reinterpret_cast<MultibootMemory *>(reinterpret_cast<std::uintptr_t>(memory) + memory->size + 4))
    {
        if (memory->type == MEMORY_TYPE_FREE)
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
        if (memory->type != MEMORY_TYPE_FREE)
        {
            node->address = memory->address;
            node->size = memory->length;
            node->next = node + 1;
            node = node->next;
        }
    }

    (node - 1)->next = nullptr;

    // carve hole for kernel space
    for (MemoryNode *n = list_free; n; n = n->next)
    {
        if (n->address < reinterpret_cast<std::uintptr_t>(kernel_area_begin) && n->address + n->size > reinterpret_cast<std::uintptr_t>(kernel_area_begin))
        {
            if (n->address < reinterpret_cast<std::uintptr_t>(kernel_area_end) && n->address + n->size > reinterpret_cast<std::uintptr_t>(kernel_area_end))
            {
                MemoryNode *split = reinterpret_cast<MemoryNode *>(memory_alloc(sizeof(MemoryNode)));

                split->address = reinterpret_cast<std::uintptr_t>(kernel_area_end);
                split->size = n->size - (reinterpret_cast<std::uintptr_t>(kernel_area_begin) - n->address) - (reinterpret_cast<std::uintptr_t>(kernel_area_end) - reinterpret_cast<std::uintptr_t>(kernel_area_begin));
                split->next = list_free;

                list_free = split;

                num_free++;

            }

            n->size = reinterpret_cast<std::uintptr_t>(kernel_area_begin) - n->address;
        }
        else
        {
            if (n->address < reinterpret_cast<std::uintptr_t>(kernel_area_end) && n->address + n->size > reinterpret_cast<std::uintptr_t>(kernel_area_end))
            {
                n->size -= reinterpret_cast<std::uintptr_t>(kernel_area_end) - n->address;
                n->address = reinterpret_cast<std::uintptr_t>(kernel_area_end);
            }
        }
    }
}

void *memory_alloc(std::size_t size)
{
    MemoryNode empty;
    MemoryNode *node = &empty;

    empty.address = 0;
    size += sizeof(MemoryNode);

    for (MemoryNode *n = list_free; n; n = n->next)
    {
        if (n->size > size && n->address > node->address && n->address + size < -1U)
            node = n;
    }

    if (&empty == node)
        return nullptr;
    else
    {
        MemoryNode *used = reinterpret_cast<MemoryNode *>(static_cast<std::uint32_t>(node->address));

        used->address = node->address + sizeof(MemoryNode);
        used->size = size - sizeof(MemoryNode);
        used->next = list_used;
        list_used = used;

        node->address += size;
        node->size -= size;

        return reinterpret_cast<void *>(static_cast<std::uint32_t>(used->address));
    }
}

void memory_free(void *address)
{
    if (list_used->address == reinterpret_cast<std::uint64_t>(address))
    {
        MemoryNode *free = list_used;

        list_used = free->next;
        free->next = list_free;
        list_free = free;
    }
    else
    {
        for (MemoryNode *n = list_used; n->next; n = n->next)
        {
            if (n->next->address == reinterpret_cast<std::uint64_t>(address))
            {
                MemoryNode *free = n->next;

                n->next = free->next;
                free->next = list_free;
                list_free = free;

                break;
            }
        }
    }
}

void memory_status(std::size_t *free, std::size_t *used)
{
    if (free != nullptr)
    {
        *free = 0;

        for (MemoryNode *node = list_free; node; node = node->next)
        {
            *free += node->size;
        }
    }

    if (used != nullptr)
    {
        *used = 0;

        for (MemoryNode *node = list_used; node; node = node->next)
        {
            *used += node->size;
        }
    }
}
