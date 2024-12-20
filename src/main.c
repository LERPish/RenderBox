#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdlib.h>
#include "input.h"
#include "init.h"
#include "framebuffer.h"
#include "math_utils.h"
#include "camera.h"

void line(ScreenEdge e, unsigned int color);

Camera cam = { {0.0f,0.0f,-2.0f}, 700.0f };

Edge cube[] = { { {0,0,0}, {1,0,0} },
                { {1,0,0}, {1,0,1} },
                { {1,0,1}, {0,0,1} },
                { {0,0,1}, {0,0,0} },
                { {0,0,0}, {1,0,1} },
                { {0,0,0}, {0,1,0} },
                { {1,0,0}, {1,1,0} },
                { {1,0,1}, {1,1,1} },
                { {0,0,1}, {0,1,1} },
                { {0,1,0}, {1,1,0} },
                { {1,1,0}, {1,1,1} },
                { {1,1,1}, {0,1,1} },
                { {0,1,1}, {0,1,0} },
                { {0,0,0}, {1,1,0} },
                { {0,0,1}, {0,1,0} },
                { {0,1,0}, {1,1,1} },
                { {0,0,1}, {1,1,1} },
                { {1,0,0}, {1,1,1} }
};

int main(int argc, char *argv[]) {
    initSDL();

    while (1) {
        char key = doInput();

        if (key) {
            //printf("%c\n", key);
            switch (key) {
                case 'w':
                    cam.pos.z += 0.01f;
                    break;
                case 'a':
                    cam.pos.x -= 0.01f;
                    break;
                case 's':
                    cam.pos.z -= 0.01f;
                    break;
                case 'd':
                    cam.pos.x += 0.01f;
                    break;
                case '^':
                    cam.pos.y += 0.01f;
                    break;
                case '_':
                    cam.pos.y -= 0.01f;
                    break;
            }
        }

        //ClearFramebuffer(0x000000FF);
        ClearFramebuffer(0x00000000);

        /*ScreenEdge e0 = {{100, 100}, {200, 200}};
        ScreenEdge e1 = {{200, 200}, {300, 200}};
        ScreenEdge e2 = {{300, 200}, {100, 100}};

        line(e0, 0x00eb4034);
        line(e1, 0x00eb4034);
        line(e2, 0x00eb4034);*/
        for (size_t x = 0; x < sizeof(cube) / sizeof(cube[0]); x++){
            Edge curCube = cube[x];
            //printf("rendering ");
            //printf("v0: (%f, %f), v1: (%f, %f)", curCube.v0.x, curCube.v0.y, curCube.v0.x, curCube.v0.y);
            ScreenEdge nSEdge = { ProjectVert(curCube.v0, cam), ProjectVert(curCube.v1, cam) };
            line(nSEdge, 0xFFFFFFFF);
        }

        RenderFramebuffer(&app);
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
        DrawPixel(x0, y0, 0.5f, color);

        if(x0 == x1 && y0 == y1) break;

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
