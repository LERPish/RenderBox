#include "camera.h"
#include "defs.h"
#include <stdio.h>

Vec2 ProjectVert(Vec3 pos3d, Camera cam) {
    Vec3 relativePos = { pos3d.x - cam.pos.x, pos3d.y - cam.pos.y, pos3d.z - cam.pos.z };

    if (relativePos.z < 0.1f) relativePos.z = 0.1f; // Prevent extreme distortions
    //printf("relativePos: (%f, %f, %f)\n", relativePos.x, relativePos.y, relativePos.z);

    // Calculate the screen aspect ratio
    float aspect = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;

    Vec2 point = { 
        (relativePos.x / (relativePos.z * aspect)) * cam.f, 
        (relativePos.y / relativePos.z) * cam.f 
    };

    point.x /= (cam.f * 0.5f); // Scale to normalized device coordinates
    point.y /= (cam.f * 0.5f);

    Vec2 fPoint = { 
        (point.x * 0.5f + 0.5f) * SCREEN_WIDTH,  // Shift and scale x to [0, SCREEN_WIDTH]
        (1.0f - (point.y * 0.5f + 0.5f)) * SCREEN_HEIGHT  // Flip and shift y to [0, SCREEN_HEIGHT]
    };
    //printf("Projected point: (%f, %f)\n", fPoint.x, fPoint.y);

    return fPoint;
}
