#ifndef CAMERA_H
#define CAMERA_H
#include "math_utils.h"

typedef struct {
    Vec3 pos;
    Vec3 rotationEuler;
    Vec3 forward, up, right;
    unsigned int FOV;
    float aspectRatio;
    float nearPlane;
    float farPlane;
    Matrice4x4 viewMatrix;
    Matrice4x4 projectionMatrix;
    int isDirty;
    float speed;
} Camera;

void UpdateCameraVectors(Camera *cam);

void GetViewMatrix(Camera *cam);

void GetProjectionMatrix(Camera *cam);

Vec3 ProjectVert(Vec3 pos3d, Camera *camera);

Camera InitCamera(Vec3 pos3d, Vec3 rotationEuler, float FoV, float nearPlane, float farPlane);

#endif
