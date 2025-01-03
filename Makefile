# Compiler and flags
CC = gcc
CFLAGS = -Isrc -Iinclude `sdl2-config --cflags` -Wall -Wextra -g
LDFLAGS = `sdl2-config --libs` -lm
OUTPUT = build/renderer

# Directories
SRC_DIR = src
OBJ_DIR = build/linux
INCLUDE_DIR = include

# Source files and object files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Rules
all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -rf $(OBJ_DIR)/*.o $(OUTPUT)

