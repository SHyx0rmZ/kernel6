CXXFLAGS = -g -g3 -Wall -Wextra -Werror -std=gnu++11 -ffreestanding -nostdlib -nostartfiles -fno-leading-underscore -O0 -fno-rtti
LDFLAGS = -n

.PHONY: all clean

all:
	@mkdir -p build/src build/obj build/out
	@echo "> ruby generate"
	@ruby generate
	@make -s $(patsubst src/%.ld, build/src/%.ld, $(wildcard src/*.ld))
	@make -s $(patsubst src/%.cpp, build/src/%.cpp, $(wildcard src/*.cpp))
	@make -s $(patsubst src/%.S, build/src/%.S, $(wildcard src/*.S))
	@make -s $(patsubst src/%.cpp, build/obj/%_cpp.o, $(wildcard src/*.cpp))
	@make -s $(patsubst src/%.S, build/obj/%_S.o, $(wildcard src/*.S))
	@make -s build/out/kernel.bin
	@echo "> done"

build/out/kernel.bin: $(patsubst src/%.cpp, build/obj/%_cpp.o, $(wildcard src/*.cpp)) $(patsubst src/%.S, build/obj/%_S.o, $(wildcard src/*.S))
	@echo "> ld $@"
	@ld $(LDFLAGS) -o $@ $^ -T build/src/kernel.ld

build/src/%: src/% build/instance.yaml
	@echo "> ruby instance $<"
	@ruby instance $< $@

build/obj/%_cpp.o: build/src/%.cpp
	@echo "> g++ $<"
	@g++ $(CXXFLAGS) -c $< -o $@

build/obj/%_S.o: build/src/%.S
	@echo "> g++ $<"
	@g++ $(CXXFLAGS) -c $< -o $@

clean:
	@-rm -Rf build
