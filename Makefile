CXXFLAGS = -g -g3 -Wall -Wextra -Werror -std=gnu++11 -ffreestanding -nostdlib -nostartfiles -nostdinc++ -fno-leading-underscore -O0 -fno-rtti -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -mno-sse3 -mno-3dnow
LDFLAGS = -n

.PHONY: all clean
.SUFFIXES:

all:
	@mkdir -p build/src/kernel build/src/loader build/src/lib build/obj/kernel build/obj/loader build/src/lib build/obj/lib build/out
	@make -s build/instance.yaml
	@make -s $(patsubst src/%.ld, build/src/%.ld, $(wildcard src/*.ld))
	@make -s $(patsubst src/%.cpp, build/src/%.cpp, $(wildcard src/*/*.cpp))
	@make -s $(patsubst src/%.S, build/src/%.S, $(wildcard src/*/*.S))
	@make -s $(patsubst src/%.hpp, build/src/%.hpp, $(wildcard src/*/*.hpp))
	@make -s $(patsubst src/%, build/src/%, $(wildcard src/lib/*))
	@make -s build/out/kernel6
	@echo "> done"

build/out/kernel: $(patsubst src/%.cpp, build/obj/%_cpp.o, $(wildcard src/kernel/*.cpp)) $(patsubst src/%.S, build/obj/%_S.o, $(wildcard src/kernel/*.S)) build/out/libnukexx.x86-64.a
	@echo "> ld $@"
	@ld $(LDFLAGS) -o $@ $^ -T build/src/script_kernel.ld

build/out/libnukexx.x86.a: $(patsubst src/%.cpp, build/obj/%_x86_cpp.o, $(wildcard src/lib/*.cpp))
	@echo "> ar $@"
	@ar -rc build/out/libnukexx.x86.a $^

build/out/libnukexx.x86-64.a: $(patsubst src/%.cpp, build/obj/%_x86_64_cpp.o, $(wildcard src/lib/*.cpp))
	@echo "> ar $@"
	@ar -rc build/out/libnukexx.x86-64.a $^

build/obj/payload: build/out/kernel build/src/payload.S
	@echo "> g++ $@"
	@g++ $(CXXFLAGS) -m32 -c build/src/payload.S -o $@

build/out/loader: build/obj/payload build/obj/_udivdi3.o $(patsubst src/%.cpp, build/obj/%_cpp.o, $(wildcard src/loader/*.cpp)) $(patsubst src/%.S, build/obj/%_S.o, $(wildcard src/loader/*.S)) build/out/libnukexx.x86.a
	@echo "> ld $@"
	@ld $(LDFLAGS) -o $@ $^ -T build/src/script_loader.ld

build/out/kernel6: build/out/loader
	@echo "> cp $@"
	@cp $< $@

build/src/%: src/% build/instance.yaml
	@echo "> ruby instance $<"
	@ruby instance $< $@

build/obj/kernel/%_cpp.o: build/src/kernel/%.cpp
	@echo "> g++ $<"
	@g++ $(CXXFLAGS) -Ibuild/src/kernel -isystem build/src/lib -m64 -c $< -o $@

build/obj/kernel/%_S.o: build/src/kernel/%.S
	@echo "> g++ $<"
	@g++ $(CXXFLAGS) -Ibuild/src/kernel -isystem build/src/lib -m64 -c $< -o $@

build/obj/loader/%_cpp.o: build/src/loader/%.cpp
	@echo "> g++ $<"
	@g++ $(CXXFLAGS) -Ibuild/src/loader -isystem build/src/lib -m32 -fno-exceptions -c $< -o $@

build/obj/loader/%_S.o: build/src/loader/%.S
	@echo "> g++ $<"
	@g++ $(CXXFLAGS) -Ibuild/src/loader -isystem build/src/lib -m32 -c $< -o $@

build/obj/lib/%_x86_cpp.o: build/src/lib/%.cpp
	@echo "> g++ $<"
	@g++ $(CXXFLAGS) -Ibuild/src/lib -isystem build/src/lib -m32 -fno-exceptions -c $< -o $@

build/obj/lib/%_x86_64_cpp.o: build/src/lib/%.cpp
	@echo "> g++ $<"
	@g++ $(CXXFLAGS) -Ibuild/src/lib -isystem build/src/lib -m64 -c $< -o $@

build/obj/_udivdi3.o:
	@echo "> ar $@"
	@ar -x $$(g++ -m32 --print-libgcc-file-name) _udivdi3.o
	@mv _udivdi3.o build/obj/


build/instance.yaml: config.yaml
	@echo "> ruby generate"
	@ruby generate

clean:
	@-rm -Rf build
