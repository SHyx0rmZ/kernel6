CXXFLAGS = -g -g3 -Wall -Wextra -Werror -std=gnu++11 -ffreestanding -nostdlib -nostartfiles -nostdinc++ -fno-leading-underscore -O0 -fno-rtti -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -mno-sse3 -mno-3dnow
LDFLAGS = -n

DIRECTORIES = kernel loader lib shared
SOURCES_KERNEL = $(wildcard src/kernel/*.cpp) $(wildcard src/kernel/*.S)
SOURCES_LOADER = $(wildcard src/loader/*.cpp) $(wildcard src/loader/*.S)
SOURCES_LIBNUKEXX = $(wildcard src/lib/*.cpp) $(wildcard src/lib/*.S)
SOURCES_SHARED = $(wildcard src/shared/*.cpp) $(wildcard src/shared/*.S)
HEADERS_KERNEL = $(wildcard src/kernel/*.hpp)
HEADERS_LOADER = $(wildcard src/loader/*.hpp)
HEADERS_LIBNUKEXX = $(patsubst %.cpp, , $(patsubst %.S, , $(wildcard src/lib/*)))
HEADERS_SHARED = $(wildcard src/shared/*.hpp)
OBJECTS_KERNEL = $(patsubst src/%.cpp, build/obj/%.cpp.o, $(patsubst src/%.S, build/obj/%.S.o, $(SOURCES_KERNEL)))
OBJECTS_LOADER = $(patsubst src/%.cpp, build/obj/%.cpp.o, $(patsubst src/%.S, build/obj/%.S.o, $(SOURCES_LOADER)))
OBJECTS_LIBNUKEXX_X86 = $(patsubst src/%.cpp, build/obj/%.x86.cpp.o, $(patsubst src/%.S, build/obj/%.x86.S.o, $(SOURCES_LIBNUKEXX)))
OBJECTS_LIBNUKEXX_X64 = $(patsubst src/%.cpp, build/obj/%.x64.cpp.o, $(patsubst src/%.S, build/obj/%.x64.S.o, $(SOURCES_LIBNUKEXX)))
OBJECTS_SHARED_X86 = $(patsubst src/%.cpp, build/obj/%.x86.cpp.o, $(patsubst src/%.S, build/obj/%.x86.S.o, $(SOURCES_SHARED)))
OBJECTS_SHARED_X64 = $(patsubst src/%.cpp, build/obj/%.x64.cpp.o, $(patsubst src/%.S, build/obj/%.x64.S.o, $(SOURCES_SHARED)))
SOURCES = $(SOURCES_KERNEL) $(SOURCES_LOADER) $(SOURCES_LIBNUKEXX) $(SOURCES_SHARED)
HEADERS = $(HEADERS_KERNEL) $(HEADERS_LOADER) $(HEADERS_LIBNUKEXX) $(HEADERS_SHARED)
OBJECTS = $(OBJECTS_KERNEL) $(OBJECTS_LOADER) $(OBJECTS_LIBNUKEXX_X86) $(OBJECTS_LIBNUKEXX_X64) $(OBJECTS_SHARED_X86) $(OBJECTS_SHARED_X64)
SCRIPTS = $(wildcard src/*.ld)

.PHONY: all clean directories
.SUFFIXES:

all:
	@make -s directories
	@make -s $(patsubst src/%, build/src/%, $(HEADERS) $(SOURCES) $(SCRIPTS))
	@make -s build/out/kernel6
	@echo "> done"

# binaries, libnukexx

build/out/libnukexx.x86.a: $(OBJECTS_LIBNUKEXX_X86)
	@echo "> ar   libnukexx.x86.a"
	@ar -rc build/out/libnukexx.x86.a $^

build/out/libnukexx.x64.a: $(OBJECTS_LIBNUKEXX_X64)
	@echo "> ar   libnukexx.x64.a"
	@ar -rc build/out/libnukexx.x64.a $^

# smpinit, needs to be included in loader

build/obj/smpinit.x64.S.o: build/src/smpinit.S
	@echo "> g++  $(patsubst build/out/%, %, $@)"
	@g++ $(CXXFLAGS) -m64 -c $< -o $@

build/out/smpinit: build/obj/smpinit.x64.S.o
	@echo "> ld   smpinit"
	@ld $(LDFLAGS) -o $@ $^ -T build/src/script_smpinit.ld

build/obj/loader/smpinit.x86.S.o: build/out/smpinit build/src/loader/smpinit.S
	@echo "> g++  loader/smpinit.S"
	@g++ $(CXXFLAGS) -Ibuild/src/loader -Ibuild/src/shared/x86 -isystem build/src/lib -m32 -c build/src/loader/smpinit.S -o $@

# binaries, kernel

build/out/kernel: $(OBJECTS_KERNEL) $(OBJECTS_SHARED_X64) build/out/libnukexx.x64.a build/src/script_kernel.ld
	@echo "> ld   kernel"
	@ld $(LDFLAGS) -o $@ $^ -T build/src/script_kernel.ld

# because we need to boot the loader first
build/out/kernel6: build/out/loader
	@echo "> cp   kernel6"
	@cp $< $@

# binaries, loader

# payload wraps kernel
build/obj/payload: build/out/kernel build/src/payload.S
	@echo "> g++  $(patsubst build/obj/%, %, $@)"
	@g++ $(CXXFLAGS) -m32 -c build/src/payload.S -o $@

build/out/loader: build/obj/payload build/obj/_udivdi3.o build/obj/_umoddi3.o $(OBJECTS_LOADER) $(OBJECTS_SHARED_X86) build/out/libnukexx.x86.a build/src/script_loader.ld
	@echo "> ld   loader"
	@ld $(LDFLAGS) -o $@ $^ -T build/src/script_loader.ld

# template instantiation

build/src/shared/%.hpp: src/shared/%.hpp build/config.yml shared instance
	@echo "> ruby $(patsubst src/%,%,$<)"
	@ruby instance build/config.yml $< $@
	@ruby shared amd64 $@ $(patsubst build/src/shared/%,build/src/shared/x64/%,$@)
	@ruby shared i386 $@ $(patsubst build/src/shared/%,build/src/shared/x86/%,$@)

build/src/%: src/% build/config.yml instance
	@echo "> ruby $(patsubst src/%,%,$<)"
	@ruby instance build/config.yml $< $@

# compilation, kernel

build/obj/kernel/%.cpp.o: build/src/kernel/%.cpp
	@echo "> g++  $(patsubst build/src/%,%,$<)"
	@g++ $(CXXFLAGS) -Ibuild/src/kernel -Ibuild/src/shared/x64 -isystem build/src/lib -m64 -c $< -o $@

build/obj/kernel/%.S.o: build/src/kernel/%.S
	@echo "> g++  $(patsubst build/src/%,%,$<)"
	@g++ $(CXXFLAGS) -Ibuild/src/kernel -Ibuild/src/shared/x64 -isystem build/src/lib -m64 -c $< -o $@

# compilation, loader

build/obj/loader/%.cpp.o: build/src/loader/%.cpp
	@echo "> g++  $(patsubst build/src/%,%,$<)"
	@g++ $(CXXFLAGS) -Ibuild/src/loader -Ibuild/src/shared/x86 -isystem build/src/lib -m32 -fno-exceptions -c $< -o $@

build/obj/loader/%.S.o: build/src/loader/%.S
	@echo "> g++  $(patsubst build/src/%,%,$<)"
	@g++ $(CXXFLAGS) -Ibuild/src/loader -Ibuild/src/shared/x86 -isystem build/src/lib -m32 -c $< -o $@

# compilation, libnukexx

build/obj/lib/%.x86.cpp.o: build/src/lib/%.cpp
	@echo "> g++  $(patsubst build/src/%,%,$<)"
	@g++ $(CXXFLAGS) -Ibuild/src/shared/x86 -Ibuild/src/lib -isystem build/src/lib -m32 -fno-exceptions -c $< -o $@

build/obj/lib/%.x86.S.o: build/src/lib/%.S
	@echo "> g++  $(patsubst build/src/%,%,$<)"
	@g++ $(CXXFLAGS) -Ibuild/src/shared/x86 -Ibuild/src/lib -isystem build/src/lib -m32 -fno-exceptions -c $< -o $@

build/obj/lib/%.x64.cpp.o: build/src/lib/%.cpp
	@echo "> g++  $(patsubst build/src/%,%,$<)"
	@g++ $(CXXFLAGS) -Ibuild/src/shared/x64 -Ibuild/src/lib -isystem build/src/lib -m64 -c $< -o $@

build/obj/lib/%.x64.S.o: build/src/lib/%.S
	@echo "> g++  $(patsubst build/src/%,%,$<)"
	@g++ $(CXXFLAGS) -Ibuild/src/shared/x64 -Ibuild/src/lib -isystem build/src/lib -m64 -c $< -o $@

# compilation, shared

build/obj/shared/%.x86.cpp.o: build/src/shared/%.cpp
	@echo "> g++ $(patsubst build/src/%,%,$<)"
	@g++ $(CXXFLAGS) -Ibuild/src/shared/x86 -Ibuild/src/lib -isystem build/src/lib -m32 -fno-exceptions -c $< -o $@

build/obj/shared/%.x86.S.o: build/src/shared/%.S
	@echo "> g++ $(patsubst build/src/%,%,$<)"
	@g++ $(CXXFLAGS) -Ibuild/src/shared/x86 -Ibuild/src/lib -isystem build/src/lib -m32 -fno-exceptions -c $< -o $@

build/obj/shared/%.x64.cpp.o: build/src/shared/%.cpp
	@echo "> g++ $(patsubst build/src/%,%,$<)"
	@g++ $(CXXFLAGS) -Ibuild/src/shared/x64 -Ibuild/src/lib -isystem build/src/lib -m64 -c $< -o $@

build/obj/shared/%.x64.S.o: build/src/shared/%.S
	@echo "> g++ $(patsubst build/src/%,%,$<)"
	@g++ $(CXXFLAGS) -Ibuild/src/shared/x64 -Ibuild/src/lib -isystem build/src/lib -m64 -c $< -o $@

# extraction of 64 bit arithmetics

build/obj/_udivdi3.o:
	@echo "> ar   $(patsubst build/obj/%,%,$@)"
	@ar -x $$(g++ -m32 --print-libgcc-file-name) _udivdi3.o
	@mv _udivdi3.o build/obj/

build/obj/_umoddi3.o:
	@echo "> ar   $(patsubst build/obj/%,%,$@)"
	@ar -x $$(g++ -m32 --print-libgcc-file-name) _umoddi3.o
	@mv _umoddi3.o build/obj/


# config

build/config.yml: config.yml generate
	@echo "> ruby config.yml"
	@ruby generate $< $@

# cleaning

clean:
	@-rm -Rf build

# creating directories

directories:
	@$(foreach dir,$(DIRECTORIES),$(shell mkdir -p build/src/$(dir)))
	@$(foreach dir,$(DIRECTORIES),$(shell mkdir -p build/obj/$(dir)))
	@mkdir -p build/src/shared/x86 build/src/shared/x64
	@mkdir -p build/out
