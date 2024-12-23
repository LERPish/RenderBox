#ifndef MATH_UTILS_H
#define MATH_UTILS_H
#include "defs.h"
#include "stdlib.h"
#include <stddef.h>

typedef struct {
    float x, y, z;
} Vec3;

typedef struct {
    float x, y;
} Vec2;

typedef struct {
    Vec3 pos;
} Vertice;

typedef struct {
    Vec2 pos;
    unsigned int color;
} ScreenVert;

typedef struct {
    size_t v0, v1;
} Edge;

typedef struct {
    Vec2 v0, v1;
} ScreenEdge;

typedef struct {
    size_t v0, v1, v2;
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

Matrice3x3 MultiplyMat3(Matrice3x3 a, Matrice3x3 b);

Vec3 MultiplyMat3Vec3(Matrice3x3 mat, Vec3 vec);

float Vec3DotProduct(Vec3 a, Vec3 b);

Vec3 Vec3Cross(Vec3 a, Vec3 b);

float Vec3Magnitude(Vec3 vec);

Vec3 Vec3Normalize(Vec3 vec);

Vec3 Vec3Add(Vec3 a, Vec3 b);

Vec3 Vec3Sub(Vec3 a, Vec3 b);

Vec3 Vec3Mult(Vec3 a, Vec3 b);

Vec3 ComputeNormal(Vec3 *v0, Vec3 *v1, Vec3 *v2);

#endif
