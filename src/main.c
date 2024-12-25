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
#include "obj_parser.h"

Camera cam = { {0.0f,0.0f,-2.0f}, 700.0f };

Vec3 baseScale = {1,1,1};

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

Edge cubeE[] = {
    {0, 1}, {1, 2}, {2, 3}, {3, 0},  // Bottom edges
    {4, 5}, {5, 6}, {6, 7}, {7, 4},  // Top edges
    {0, 4}, {1, 5}, {2, 6}, {3, 7},   // Vertical edges
    {0, 5}, {1, 6}, {2, 7}, {3, 4},
    {0, 2}, {7, 5}
};

Face cubeF[] = {
    {0, 1, 2, {0, 0, 0}},
    {0, 2, 3, {0, 0, 0}},
    {4, 5, 6, {0, 0, 0}},
    {4, 6, 7, {0, 0, 0}},
    {0, 1, 5, {0, 0, 0}},
    {0, 5, 4, {0, 0, 0}},
    {2, 3, 7, {0, 0, 0}},
    {2, 7, 6, {0, 0, 0}},
    {1, 2, 6, {0, 0, 0}},
    {1, 6, 5, {0, 0, 0}},
    {3, 0, 4, {0, 0, 0}},
    {3, 4, 7, {0, 0, 0}},
};

int main(int argc, char *argv[]) {
    initSDL();

    Mesh *cMesh = LoadOBJ("./assets/Goobi.obj");
    fprintf(stdout, "Mesh Created\n");
    Object3D *obj = CreateObject3D(
        cMesh,
        (Vec3){0.0f, 0.0f, 0.0f}, // Position
        (Vec3){0.0f, 0.0f, 0.0f}, // Rotation
        (Vec3){0.2f, 0.2f, 0.2f}  // Scale
    );
    fprintf(stdout, "Object Created\n");
    int small = 0;

    while (1) {
        //printf("doInput\n");
        char key = doInput();
        //printf("got my key\n");

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
                default:
                    break;
            }
        }

        //ClearFramebuffer(0x000000FF);
        //printf("Analyzed key\n");
        ClearFramebuffer(0x000000FF);
        //printf("Cleared Buffer\n");

        RotateObject3D(obj, (Vec3){0,0.01f,0});

        /*if (obj->scale.y >= 0.06f && small == 0)
            ScaleObject3D(obj, (Vec3){1,0.99f,1});
        else {
            ScaleObject3D(obj, (Vec3){1,1.01f,1});
            if (obj->scale.y >= 0.2f) 
                small = 0;
            else
                small = 1;
        }*/

        TransformObject3D(obj);

        RenderMesh(obj->transformedMesh, cam);
        //RenderMeshWireframe(obj->transformedMesh, cam);

        RenderFramebuffer(&app);
    }
    return 0;
}
