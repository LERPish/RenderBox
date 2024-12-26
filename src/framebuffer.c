#include "framebuffer.h"
#include "camera.h"
#include "defs.h"
#include "math_utils.h"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <math.h>
#include <stdlib.h>

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
        if (zbuffer[y][x] >= z) {
            framebuffer[y][x] = color;
            zbuffer[y][x] = z;
        }
        return 1;
    }
    return 0;
}

void line(Vec2 v0, Vec2 v1, unsigned int color) {
    int x0 = v0.x;
    int y0 = v0.y;
    int x1 = v1.x;
    int y1 = v1.y;

    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        int ret = DrawPixel(x0, y0, -10.0f, color);

        if((x0 == x1 && y0 == y1) || ret == 0) break;

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

int ComputeBarycentric(Vec3 v0, Vec3 v1, Vec3 v2, int x, int y, float *lambda1, float *lambda2, float *lambda3) {
    // Full triangle area
    float aTri = (v1.x - v0.x) * (v2.y - v0.y) - (v2.x - v0.x) * (v1.y - v0.y);
    if (fabs(aTri) < 1e-6) return 0; // Degenerate triangle

    // Sub-triangle areas
    float a1 = (v1.x - x) * (v2.y - y) - (v2.x - x) * (v1.y - y);
    float a2 = (v2.x - x) * (v0.y - y) - (v0.x - x) * (v2.y - y);
    float a3 = (v0.x - x) * (v1.y - y) - (v1.x - x) * (v0.y - y);

    // Normalize
    *lambda1 = a1 / aTri;
    *lambda2 = a2 / aTri;
    *lambda3 = a3 / aTri;

    // Check if inside the triangle
    return (*lambda1 >= 0 && *lambda2 >= 0 && *lambda3 >= 0 && 
            fabs(*lambda1 + *lambda2 + *lambda3 - 1.0f) < 1e-6);
}



void RasterizeTriangle (Vec3 v0, Vec3 v1, Vec3 v2, Camera cam, unsigned int color) {
    v0 = ProjectVert(v0, &cam);
    v1 = ProjectVert(v1, &cam);
    v2 = ProjectVert(v2, &cam);

    int minX = floorf(fminf(v0.x, fminf(v1.x, v2.x)));
    int maxX = ceilf(fmaxf(v0.x, fmaxf(v1.x, v2.x)));
    int minY = floorf(fminf(v0.y, fminf(v1.y, v2.y)));
    int maxY = ceilf(fmaxf(v0.y, fmaxf(v1.y, v2.y)));

    minX = fmax(minX, 0);
    maxX = fmin(maxX, SCREEN_WIDTH - 1);
    minY = fmax(minY, 0);
    maxY = fmin(maxY, SCREEN_HEIGHT - 1);

    //printf("Triangle Bounding Box: minX=%d, maxX=%d, minY=%d, maxY=%d\n", minX, maxX, minY, maxY);
    
    for (int y = minY; y <= maxY; y++) {
        for (int x = minX; x <= maxX; x++) {
            float lambda1, lambda2, lambda3;
            if (ComputeBarycentric(v0,v1,v2,x,y,&lambda1,&lambda2,&lambda3)){
                //printf("%f, %f, %f\n", lambda1, lambda2, lambda3);
                float depth = lambda1 * v0.z + lambda2 * v1.z + lambda3 * v2.z;
                //printf("depth: %f\n", depth);

                DrawPixel(x, y, depth, color);
            }
        }
    }
}
