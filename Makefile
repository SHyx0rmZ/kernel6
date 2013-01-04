CXXFLAGS = -g -g3 -Wall -Wextra -Werror -std=gnu++11 -ffreestanding -nostdlib -nostartfiles -nostdinc++ -fno-leading-underscore -O0 -fno-rtti -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -mno-sse3 -mno-3dnow
LDFLAGS = -n

.PHONY: all clean

all:
	@mkdir -p build/src/kernel build/src/loader build/src/lib build/obj/kernel build/obj/loader build/src/lib build/out
	@echo "> ruby generate"
	@ruby generate
	@make -s $(patsubst src/%.ld, build/src/%.ld, $(wildcard src/*.ld))
	@make -s $(patsubst src/%.cpp, build/src/%.cpp, $(wildcard src/*/*.cpp))
	@make -s $(patsubst src/%.S, build/src/%.S, $(wildcard src/*/*.S))
	@make -s $(patsubst src/%.hpp, build/src/%.hpp, $(wildcard src/*/*.hpp))
	@make -s $(patsubst src/%, build/src/%, $(wildcard src/lib/*))
	@make -s build/out/kernel6
	@echo "> done"

build/out/kernel: $(patsubst src/%.cpp, build/obj/%_cpp.o, $(wildcard src/kernel/*.cpp)) $(patsubst src/%.S, build/obj/%_S.o, $(wildcard src/kernel/*.S))
	@echo "> ld $@"
	@ld $(LDFLAGS) -o $@ $^ -T build/src/script_kernel.ld

build/obj/payload: build/out/kernel build/src/payload.S
	@echo "> g++ $@"
	@g++ $(CXXFLAGS) -m32 -c build/src/payload.S -o $@

build/out/loader: build/obj/payload $(patsubst src/%.cpp, build/obj/%_cpp.o, $(wildcard src/loader/*.cpp)) $(patsubst src/%.S, build/obj/%_S.o, $(wildcard src/loader/*.S))
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

clean:
	@-rm -Rf build
