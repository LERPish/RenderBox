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

void line(ScreenEdge e, unsigned int color) {
    int x0 = e.v0.x;
    int y0 = e.v0.y;
    int x1 = e.v1.x;
    int y1 = e.v1.y;

    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        int ret = DrawPixel(x0, y0, 0.5f, color);

        if((x0 == x1 && y0 == y1) /*|| ret == 0*/) break;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}
