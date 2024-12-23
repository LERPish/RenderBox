#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input.h"
#include "init.h"
#include "framebuffer.h"
#include "math_utils.h"
#include "entities.h"
#include "camera.h"

void line(ScreenEdge e, unsigned int color);

Camera cam = { {0.0f,0.0f,-2.0f}, 700.0f };

Vec3 baseScale = {1,1,1};


/*Edge cubeE[] = { { {0,0,0}, {1,0,0} },
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
};*/

Vertice cubeV[] = {
    (Vertice){.pos = {-0.5f, -0.5f, -0.5f}},
    (Vertice){.pos = { 0.5f, -0.5f, -0.5f}},
    (Vertice){.pos = { 0.5f,  0.5f, -0.5f}},
    (Vertice){.pos = {-0.5f,  0.5f, -0.5f}},
    (Vertice){.pos = {-0.5f, -0.5f,  0.5f}},
    (Vertice){.pos = { 0.5f, -0.5f,  0.5f}},
    (Vertice){.pos = { 0.5f,  0.5f,  0.5f}},
    (Vertice){.pos = {-0.5f,  0.5f,  0.5f}}
};

Edge cubeE[18]; 
Face cubeF[12];

Edge cubeE[] = {
    {0, 1}, {1, 2}, {2, 3}, {3, 0},  // Bottom edges
    {4, 5}, {5, 6}, {6, 7}, {7, 4},  // Top edges
    {0, 4}, {1, 5}, {2, 6}, {3, 7},   // Vertical edges
    {0, 5}, {1, 6}, {2, 7}, {3, 4},
    {0, 2}, {7, 5}
};

void InitializeCubeFaces() {
    cubeF[0] = (Face){&cubeV[0], &cubeV[1], &cubeV[2], {0, 0, 0}};
    cubeF[1] = (Face){&cubeV[0], &cubeV[2], &cubeV[3], {0, 0, 0}};
    cubeF[2] = (Face){&cubeV[4], &cubeV[5], &cubeV[6], {0, 0, 0}};
    cubeF[3] = (Face){&cubeV[4], &cubeV[6], &cubeV[7], {0, 0, 0}};
    cubeF[4] = (Face){&cubeV[0], &cubeV[1], &cubeV[5], {0, 0, 0}};
    cubeF[5] = (Face){&cubeV[0], &cubeV[5], &cubeV[4], {0, 0, 0}};
    cubeF[6] = (Face){&cubeV[2], &cubeV[3], &cubeV[7], {0, 0, 0}};
    cubeF[7] = (Face){&cubeV[2], &cubeV[7], &cubeV[6], {0, 0, 0}};
    cubeF[8] = (Face){&cubeV[1], &cubeV[2], &cubeV[6], {0, 0, 0}};
    cubeF[9] = (Face){&cubeV[1], &cubeV[6], &cubeV[5], {0, 0, 0}};
    cubeF[10] = (Face){&cubeV[3], &cubeV[0], &cubeV[4], {0, 0, 0}};
    cubeF[11] = (Face){&cubeV[3], &cubeV[4], &cubeV[7], {0, 0, 0}};
}

int main(int argc, char *argv[]) {
    initSDL();
    InitializeCubeFaces();

    Mesh *cMesh = CreateMesh((size_t)8, (size_t)18, (size_t)12);
    memcpy(cMesh->vertices, cubeV, sizeof(cubeV));
    memcpy(cMesh->edges, cubeE, sizeof(cubeE));
    memcpy(cMesh->faces, cubeF, sizeof(cubeF));
    fprintf(stdout, "Mesh Created\n");
    //AddVertsToMesh(cMesh, cubeV);
    //AddEdgesToMesh(cMesh, cubeE);
    Object3D *obj = CreateObject3D(
        cMesh,
        (Vec3){0.0f, 0.0f, 0.0f}, // Position
        (Vec3){0.0f, 0.0f, 0.0f}, // Rotation
        (Vec3){1.0f, 1.0f, 1.0f}  // Scale
    );
    fprintf(stdout, "Object Created\n");
    int small = 0;

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

        RotateObject3D(obj, (Vec3){0,0.001f,0});
        if (obj->scale.y >= 0.3f && small == 0)
            ScaleObject3D(obj, (Vec3){1,0.999f,1});
        else {
            ScaleObject3D(obj, (Vec3){1,1.001f,1});
            if (obj->scale.y >= 1.0f) 
                small = 0;
            else
                small = 1;
        }
        TransformObject3D(obj);
        printf("Object Position: (%.2f, %.2f, %.2f)\n", obj->position.x, obj->position.y, obj->position.z);
        printf("Object Rotation: (%.2f, %.2f, %.2f)\n", obj->eulerRotation.x, obj->eulerRotation.y, obj->eulerRotation.z);
        printf("Object Scale:    (%.2f, %.2f, %.2f)\n", obj->scale.x, obj->scale.y, obj->scale.z);

        //printf("rendering\n");

        for (size_t x = 0; x < obj->mesh->edge_count; x++){
            Edge curCube = obj->transformedMesh->edges[x];
            Vec3 v0 = obj->transformedMesh->vertices[curCube.v0].pos;
            Vec3 v1 = obj->transformedMesh->vertices[curCube.v1].pos;

            ScreenEdge nSEdge = { ProjectVert(v0, cam), ProjectVert(v1, cam) };

            /*printf("Edge %zu: v0 (%p), v1 (%p)\n", x, curCube.v0, curCube.v1);
            printf("v0 pos: (%f, %f, %f), v1 pos: (%f, %f, %f)\n",
                    curCube.v0->pos.x, curCube.v0->pos.y, curCube.v0->pos.z,
                    curCube.v1->pos.x, curCube.v1->pos.y, curCube.v1->pos.z);*/

            line(nSEdge, 0xFFFFFFFF);
        }
        //printf("finished the loop\n");

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
