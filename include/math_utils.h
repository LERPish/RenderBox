#ifndef MATH_UTILS_H
#define MATH_UTILS_H
#include "defs.h"

typedef struct {
    float x, y, z;
} Vec3;

typedef struct {
    float x, y;
} Vec2;

typedef struct {
    Vec3 pos;
    unsigned int color;
} Vertex;

typedef struct {
    Vec2 pos;
    unsigned int color;
} ScreenVert;

typedef struct {
    Vec3 v0, v1;
} Edge;

typedef struct {
    Vec2 v0, v1;
} ScreenEdge;

typedef struct {
    Vec3 v0, v1, v2;
    Vec3 normal;
} Face;

typedef struct {
    float M[4][4];
} Matrice4x4;

typedef struct {
    float M[3][3];
} Matrice3x3;

Matrice3x3 RxAxis(float theta);
Matrice3x3 RyAxis(float theta);
Matrice3x3 RzAxis(float theta);

Vec3 MultiplyMat3Vec3(Matrice3x3 mat, Vec3 vec);

float Vec3DotProduct(Vec3 a, Vec3 b);

#endif
