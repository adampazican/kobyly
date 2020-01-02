BUILD_DIR=build
COMMON_FLAGS=-std=c++11 -Wall -o build/o
DEBUG_FLAGS=-O0 -ggdb -fsanitize=address -fno-omit-frame-pointer -DDEBUG_BUILD=1 -pthread
RELEASE_FLAGS=-Werror -O3
UNITS=main.cpp

.DEFAULT_GOAL := all

clean:
	rm build/*

debug:
	g++ $(DEBUG_FLAGS) $(COMMON_FLAGS) $(UNITS)

release:
	g++ $(RELEASE_FLAGS) $(COMMON_FLAGS) $(UNITS)

all: release
	

$(shell mkdir -p $(BUILD_DIR))