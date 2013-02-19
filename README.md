kernel6
=======

Operating system for x86-64 cpus, written in C++11

OS is split between 32 bit intermediate loader which sets up a proper
environment and 64 bit kernel, which will be instanced per logical CPU
by the loader. Kernel thus boots with full (to the extent needed) support
of C++11 runtime and SMP.
