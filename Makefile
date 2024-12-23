# Variables
CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -g
LDFLAGS = -lSDL2 -lm
SRC = src/main.c src/init.c src/input.c src/framebuffer.c src/math_utils.c src/camera.c src/entities.c
OBJ = $(SRC:src/%.c=build/%.o)
OUT = build/renderer

# Rules
all: $(OUT)

$(OUT): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

build/%.o: src/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f build/*.o $(OUT)

