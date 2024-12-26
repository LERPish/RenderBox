#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_video.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "input.h"
#include "init.h"
#include "math_utils.h"
#include "camera.h"
#include "framebuffer.h"
#include "entities.h"
#include "obj_parser.h"

int main(int argc, char *argv[]) {
    initSDL();
    SDL_WarpMouseInWindow(app.window, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    SDL_SetWindowGrab(app.window, SDL_TRUE);
    SDL_ShowCursor(0);
    Camera cam = InitCamera((Vec3){0,0,0}, (Vec3){0,0,0}, 60.0f, 0.1f, 1000.0f);
    Mesh *cMesh;

    if (argc < 2) {
        cMesh = LoadOBJ("./assets/cube.obj");
    } else {
        cMesh = LoadOBJ(argv[1]);
    }
    fprintf(stdout, "Mesh Created\n");
    Object3D *obj = CreateObject3D(
        cMesh,
        (Vec3){0.0f, 0.0f, 10.0f},
        (Vec3){0.0f, 0.0f, 0.0f},
        (Vec3){1.0f, 1.0f, 1.0f}
    );
    fprintf(stdout, "Object Created\n");
    int small = 0;

    while (1) {
        doInput(&cam, app.window);

        cam.isDirty = 1;

        if (cam.isDirty) {
            UpdateCameraVectors(&cam);
            GetViewMatrix(&cam);
        }

        ClearFramebuffer(0x000000FF);

        RotateObject3D(obj, (Vec3){0,0.01f,0});

        TransformObject3D(obj);

        RenderMesh(obj->transformedMesh, cam);
        //RenderMeshWireframe(obj->transformedMesh, cam);
        //ProjectVert((Vec3){0,0,-1},&cam);

        RenderFramebuffer(&app);
    }
    return 0;
}
