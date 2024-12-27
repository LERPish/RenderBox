#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mutex.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_video.h>
#include "camera.h"
#include "math_utils.h"
#include "input.h"

void doInput(Camera *cam) {
    SDL_Event event;
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    //char key;
    int xRel = 0, yRel = 0;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                exit(0);
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        exit(0);
                        break;
                    default:
                        break;
                }
                break;
            case SDL_MOUSEMOTION:
                cam->isDirty = 1;
                xRel = event.motion.xrel;
                yRel = event.motion.yrel;
                break;
            case SDL_MOUSEBUTTONDOWN:
                SDL_SetRelativeMouseMode(SDL_FALSE);
                if(cam->speed == 0) {
                    cam->speed = 1;
                } else {
                    cam->speed = 0;
                }
                break;
            default:
                break;
        }
    }

    // Handle mouse movement for camera rotation
    /*if (cam->speed == 0) {
        cam->rotationEuler.y -= xRel * 0.5f; // Adjust sensitivity as needed
    } else {
        cam->rotationEuler.x -= yRel * 0.5f;
    }*/
    cam->rotationEuler.y += xRel * 0.25f; // Adjust sensitivity as needed
    cam->rotationEuler.x -= yRel * 0.25f;
    //cam->rotationEuler.z = 0.0f;

    // Clamp pitch to avoid flipping
    if (cam->rotationEuler.x > 89.0f) cam->rotationEuler.x = 89.0f;
    if (cam->rotationEuler.x < -89.0f) cam->rotationEuler.x = -89.0f;

    // Calculate local movement vector
    Vec3 localMovement = {0.0f, 0.0f, 0.0f};
    if (state[SDL_SCANCODE_W]) localMovement = Vec3Add(localMovement, cam->forward); // Forward
    if (state[SDL_SCANCODE_S]) localMovement = Vec3Sub(localMovement, cam->forward); // Backward
    if (state[SDL_SCANCODE_A]) localMovement = Vec3Sub(localMovement, cam->right);   // Left
    if (state[SDL_SCANCODE_D]) localMovement = Vec3Add(localMovement, cam->right);   // Right
    if (state[SDL_SCANCODE_E]) localMovement = Vec3Add(localMovement, cam->up);      // Up
    if (state[SDL_SCANCODE_Q]) localMovement = Vec3Sub(localMovement, cam->up);      // Down

    if (Vec3Magnitude(localMovement) != 0) {
        cam->isDirty = 1;
        // Normalize and scale the movement vector
        localMovement = Vec3Normalize(localMovement);
        localMovement = (Vec3){localMovement.x * 0.1f,
            localMovement.y * 0.1f,
            localMovement.z * 0.1f
        }; // Adjust speed as needed

        //printf("%f,%f,%f\n", localMovement.x, localMovement.y, localMovement.z);

        cam->pos = Vec3Add(cam->pos, localMovement);
    }
}
