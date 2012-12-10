CXXFLAGS = -g -g3 -Wall -Wextra -Werror -std=gnu++11 -ffreestanding -nostdlib -nostartfiles -fno-leading-underscore -O0 -fno-rtti

.PHONY: all clean

all:
	@mkdir -p build/src build/obj
	@echo "> ruby generate"
	@ruby generate
	@make -s $(patsubst build/src/%.cpp, build/obj/%.o, $(wildcard build/src/*.cpp))
	@echo "> done"

build/obj/%.o: build/src/%.cpp
	@echo "> g++ $^"
	@g++ $(CXXFLAGS) -c $^ -o $@

clean:
	@-rm -Rf build
