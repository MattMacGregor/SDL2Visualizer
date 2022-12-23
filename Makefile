CC = g++
CFLAGS = -Wall -std=c++17

SOURCES := $(wildcard src/*.cpp)
INCLUDES = include/ include/glm/

OBJECTS = $(SOURCES:src/%.cpp=%.o)
LIBS = fftw3 SDL2 dl assimp m
all: demo/demo

demo/demo: $(OBJECTS)
	$(CC) -Llib/ $(LIBS:%=-l%) -o $@ $(OBJECTS)

%.o : src/%.cpp
	echo $(CC) $(INCLUDES:%=-I%) $(CFLAGS) -c $< -o $@
	@$(CC) $(INCLUDES:%=-I%) $(CFLAGS) -c $< -o $@

clean:
	rm $(wildcard *.o)

run:
	LD_LIBRARY_PATH=lib/ demo/demo
