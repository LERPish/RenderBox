#include "framebuffer.h"
#include "defs.h"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>

unsigned int framebuffer[SCREEN_HEIGHT][SCREEN_WIDTH];
float zbuffer[SCREEN_HEIGHT][SCREEN_WIDTH];

void RenderFramebuffer(App *app) {
    SDL_Renderer *renderer = app->renderer;

    SDL_Texture *texture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            SCREEN_WIDTH, SCREEN_HEIGHT
    );

    SDL_UpdateTexture(texture, NULL, framebuffer, SCREEN_WIDTH * sizeof(unsigned int));

    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    SDL_DestroyTexture(texture);
}

void ClearFramebuffer(unsigned int color) {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            framebuffer[y][x] = color;
            zbuffer[y][x] = 1.0f;
        }
    }
}

int DrawPixel(int x, int y, float z, unsigned int color) {
    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
        if (zbuffer[y][x] > z) {
            framebuffer[y][x] = color;
            zbuffer[y][x] = z;
        }
        return 1;
    }
    return 0;
}
