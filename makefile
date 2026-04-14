# Makefile for Linux - HELIUM3_rts

TARGET = helium3

# Foldery z plikami źródłowymi (.cpp)
SRC_DIRS = src src/entities src/core src/systems src/ui

# Foldery, w których szukamy plików nagłówkowych (.h)
INCLUDE_DIRS = rc src/entities src/core src/systems src/ui

SOURCES = $(wildcard $(addsuffix /*.cpp, $(SRC_DIRS)))
OBJECTS = $(SOURCES:.cpp=.o)

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -g

# SDL2 + SDL2_ttf
SDL_CFLAGS := $(shell pkg-config --cflags sdl2 SDL2_ttf)
SDL_LIBS   := $(shell pkg-config --libs sdl2 SDL2_ttf) -lSDL2main

# Dodajemy ścieżki do nagłówków
INCLUDES = $(SDL_CFLAGS) $(addprefix -I, $(INCLUDE_DIRS))

LDFLAGS  = $(SDL_LIBS)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)

run: all
	./$(TARGET)