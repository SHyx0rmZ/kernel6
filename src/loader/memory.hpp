#ifndef _LOADER_MEMORY_HPP_
#define _LOADER_MEMORY_HPP_

#include <cstddef>

#include "multiboot.hpp"

#define MEMORY_TYPE_FREE        1
#define MEMORY_TYPE_RESERVED    2
#define MEMORY_TYPE_RECLAIMABLE 3
#define MEMORY_TYPE_ACPI        4
#define MEMORY_TYPE_BAD         5

void memory_init(MultibootInfo *info);
void *memory_alloc(std::size_t size);
void memory_free(void *address);

#endif
