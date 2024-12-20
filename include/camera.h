#ifndef CAMERA_H
#define CAMERA_H
#include "math_utils.h"

typedef struct {
    Vec3 pos;
    float f;
} Camera;

Vec2 ProjectVert(Vec3 pos3d, Camera camera);

#endif
