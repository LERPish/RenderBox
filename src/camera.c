#include "camera.h"
#include "defs.h"
#include <math.h>
#include "math_utils.h"
#include <stdio.h>

void UpdateCameraVectors(Camera *cam) {
    float pitch = cam->rotationEuler.x * (M_PI / 180.0f);
    float yaw = cam->rotationEuler.y * (M_PI / 180.0f);
    float roll = cam->rotationEuler.z * (M_PI / 180.0f);

    // Generate rotation matrices for each axis
    Matrice3x3 rotationPitch = RxAxis(pitch);
    Matrice3x3 rotationYaw = RyAxis(yaw);
    Matrice3x3 rotationRoll = RzAxis(roll);

    // Combine matrices in the order: Roll -> Pitch -> Yaw
    Matrice3x3 rotation = MultiplyMat3(rotationYaw, MultiplyMat3(rotationPitch, rotationRoll));

    cam->right = Vec3Normalize((Vec3){ rotation.M[0][0], rotation.M[0][1], rotation.M[0][2] });
    cam->up = Vec3Normalize((Vec3){ rotation.M[1][0], rotation.M[1][1], rotation.M[1][2] });
    cam->forward = Vec3Normalize((Vec3){ rotation.M[2][0], rotation.M[2][1], rotation.M[2][2] });

    printf("Rotation Matrix:\n");
    for (int i = 0; i < 3; i++) {
        printf("[%f, %f, %f]\n", rotation.M[i][0], rotation.M[i][1], rotation.M[i][2]);
    }

    printf("Right: (%f, %f, %f)\n", cam->right.x, cam->right.y, cam->right.z);
    printf("Up: (%f, %f, %f)\n", cam->up.x, cam->up.y, cam->up.z);
    printf("Forward: (%f, %f, %f)\n", cam->forward.x, cam->forward.y, cam->forward.z);
    printf("\n");
}


void GetViewMatrix(Camera *cam) {
    Vec3 up = cam->up;
    Vec3 forward = cam->forward;
    Vec3 right = cam->right;
    Vec3 pos = cam->pos;
    Vec3 target = Vec3Add(pos, forward);

    Matrice4x4 viewMat = MatPointAt(forward, right, up, pos);
    viewMat = MatQuickInverse(viewMat);

    /*printf("View Matrix:\n");
    for (int i = 0; i < 4; i++) {
        printf("[%f, %f, %f, %f]\n", viewMat.M[i][0], viewMat.M[i][1], viewMat.M[i][2], viewMat.M[i][3]);
    }
    printf("\n");


    printf("View Matrix Inv:\n");
    for (int i = 0; i < 4; i++) {
        printf("[%f, %f, %f, %f]\n", viewMat.M[i][0], viewMat.M[i][1], viewMat.M[i][2], viewMat.M[i][3]);
    }
    printf("\n");*/

    cam->viewMatrix = viewMat;
    cam->isDirty = 0;
}

void GetProjectionMatrix(Camera *cam) {
    float FoV = (cam->FOV) * (M_PI/180.0f);
    float f = 1.0f / tanf(FoV);
    float a = 1.0f / cam->aspectRatio;
    float near = cam->nearPlane;
    float far = cam->farPlane;
    float q = (far)/(far - near);
    float q1 = (-far*near)/(far - near);
    printf("%f\n",f);

    Matrice4x4 projMat = {.M = {
        {0.0f,0.0f,0.0f,0.0f},
        {0.0f,0.0f,0.0f,0.0f},
        {0.0f,0.0f,0.0f,0.0f},
        {0.0f,0.0f,0.0f,0.0f}}
    };
    projMat.M[0][0] = a*f;
    projMat.M[1][1] = f;
    projMat.M[2][2] = q;
    projMat.M[2][3] = 1.0f;
    projMat.M[3][2] = q1;
    projMat.M[3][3] = 0.0f;

    cam->projectionMatrix = projMat;
}

Vec3 ProjectVert(Vec3 pos3d, Camera *cam) {
    Matrice4x4 projectMat = cam->projectionMatrix;
    Vec4 view_space = MultiplyMat4Vec4(cam->viewMatrix, (Vec4){ pos3d.x, pos3d.y, pos3d.z, 1.0f });
    //view_space.w = 1.0f;
    /*printf("view point: (%f, %f, %f)\n\n", view_space.x, view_space.y, view_space.z);
    printf("Project Matrix:\n");
    for (int i = 0; i < 4; i++) {
        printf("[%f, %f, %f, %f]\n", projectMat.M[i][0], projectMat.M[i][1], projectMat.M[i][2], projectMat.M[i][3]);
    }
    printf("\n");*/
    Vec4 clip_space = MultiplyMat4Vec4(projectMat, view_space);
    //Vec4 clip_space = MultiplyMat4Vec4(projectMat, view_space);
    //printf("clip space: (%f,%f,%f,%f)\n", clip_space.x, clip_space.y, clip_space.z, clip_space.w);
    if (clip_space.w <= 0) {
        //printf("oh no\n");
        return (Vec3){-1000,0,0};
    }

    Vec3 ndc = (Vec3){
        clip_space.x / clip_space.w,
        clip_space.y / clip_space.w,
        clip_space.z / clip_space.w
    };

    Vec3 screenPoint = {
        (ndc.x * 0.5f + 0.5f) * (float)SCREEN_WIDTH,
        (1.0f - (ndc.y * 0.5f + 0.5f)) * (float)SCREEN_HEIGHT,
        -((ndc.z * 0.5F + 0.5F) / cam->farPlane)
    };
    //printf("screen point: (%f, %f, %f)\n\n", screenPoint.x, screenPoint.y, screenPoint.z);

    return screenPoint;
}

Camera InitCamera(Vec3 pos3d, Vec3 rotationEuler, float FoV, float nearPlane, float farPlane) {
    Camera cam;
    cam.pos = pos3d;
    cam.FOV = FoV;
    cam.nearPlane = nearPlane;
    cam.farPlane = farPlane;
    cam.aspectRatio = (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT;
    //cam.isDirty = 1;

    cam.rotationEuler = rotationEuler;

    UpdateCameraVectors(&cam);

    GetViewMatrix(&cam);
    GetProjectionMatrix(&cam);

    return cam;
}
