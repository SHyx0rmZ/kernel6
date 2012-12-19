.set LOADER_STACK_SIZE, <%= config["loader_stack_size"] %>
.set LOADER_MAP_SIZE, <%= config["loader_map_size"] %><% exit 1 if config["loader_map_size"] > 0x40000000 %>
.set LOADER_STACK_ADDRESS, <%= config["loader_stack_address"] %>
.set LOADER_PML4_ADDRESS, <%= config["loader_pml4_address"] %>

.section .multiboot

.global LOADER_STACK_SIZE
.global LOADER_MAP_SIZE
.global LOADER_PML4_ADDRESS

#define LOADER_MB_MAGIC 0x1badb002
#define LOADER_MB_FLAGS 0x0003
#define LOADER_MB_CHECKSUM -(LOADER_MB_MAGIC + LOADER_MB_FLAGS)

.align 4

.int LOADER_MB_MAGIC
.int LOADER_MB_FLAGS
.int LOADER_MB_CHECKSUM

/* unused */
.int 0
.int 0
.int 0
.int 0
.int 0

/* graphics mode 1, 80x25 chars, automatically select depth */
.int 1
.int 80
.int 25
.int 0

<% if config["loader_use_bss"] %>

/* provide extra space for stack and page tables */
.section .loader, "aw", @nobits

.align 0x1000
.space LOADER_STACK_SIZE
.set LOADER_STACK_ADDRESS, .
.set LOADER_PML4_ADDRESS, .
.space 0xb000

<% end %>

#if LOADER_MAP_SIZE < LOADER_STACK_ADDRESS
.set LOADER_MAP_SIZE, LOADER_STACK_ADDRESS
#endif

.section .text
.code32
