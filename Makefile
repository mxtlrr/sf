CC := g++

CFLAGS  := -Wall -Wextra -Os -g -Iinclude/ $(LDFLAGS)
LDFLAGS := -lm

TARGET := bin/sf
override OFILES := $(shell find ./obj/ -type f -name '*.o')
override CFILES := $(shell find ./src/ -type f -name '*.cpp')

all: build
.PHONY: all

build:
	@mkdir -p bin/ obj/
	@$(foreach file, $(CFILES), $(CC) $(CFLAGS) -c $(file) -o obj/$(basename $(notdir $(file))).o;echo CC $(file);)
	@$(CC) $(OFILES) $(LDFLAGS) -o $(TARGET)
	@echo LD $(TARGET)

clean:
	rm -rf bin/