#include "math_utils.h"
#include <math.h>

Matrice3x3 RxAxis(float theta) {
    float c = cosf(theta);
    float s = sinf(theta);
    Matrice3x3 ret = {{{1,0,0},
                       {0,c,-s},
                       {0,s,c}}};
    return ret;
}

Matrice3x3 RyAxis(float theta) {
    float c = cosf(theta);
    float s = sinf(theta);
    Matrice3x3 ret = {{{c,0,s},
                       {0,1,0},
                       {-s,0,c}}};
    return ret;
}

Matrice3x3 RzAxis(float theta) {
    float c = cosf(theta);
    float s = sinf(theta);
    Matrice3x3 ret = {{{c,-s,0},
                       {s,c,0},
                       {0,0,1}}};
    return ret;
}

Matrice3x3 MultiplyMat3(Matrice3x3 a, Matrice3x3 b) {
    Matrice3x3 result = {0};

    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            result.M[row][col] =
                a.M[row][0] * b.M[0][col] +
                a.M[row][1] * b.M[1][col] +
                a.M[row][2] * b.M[2][col];
        }
    }

    return result;
}

Vec3 MultiplyMat3Vec3(Matrice3x3 mat, Vec3 vec) {
    Vec3 ret = {mat.M[0][0] * vec.x + mat.M[0][1] * vec.y + mat.M[0][2] * vec.z,
                mat.M[1][0] * vec.x + mat.M[1][1] * vec.y + mat.M[1][2] * vec.z,
                mat.M[2][0] * vec.x + mat.M[2][1] * vec.y + mat.M[2][2] * vec.z};
    return ret;
}

float Vec3DotProduct(Vec3 a, Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 Vec3Cross(Vec3 a, Vec3 b) {
    float c0 = (a.y * b.z) - (a.z * b.y);
    float c1 = (a.z * b.x) - (a.x * b.z);
    float c2 = (a.x * b.y) - (a.y * b.x);
    return (Vec3){ c0, c1, c2 };
}

float Vec3Magnitude(Vec3 vec) {
    float v0 = powf(vec.x, 2);
    float v1 = powf(vec.y, 2);
    float v2 = powf(vec.z, 2);
    return sqrtf(v0 + v1 + v2);
}

Vec3 Vec3Normalize(Vec3 vec) {
    float magnitude = Vec3Magnitude(vec);
    return (Vec3){vec.x/magnitude, vec.y/magnitude, vec.z/magnitude};
}

Vec3 Vec3Add(Vec3 a, Vec3 b) {
    Vec3 res = {a.x + b.x, a.y + b.y, a.z + b.z};
    return res;
}

Vec3 Vec3Sub(Vec3 a, Vec3 b) {
    Vec3 res = {a.x - b.x, a.y - b.y, a.z - b.z};
    return res;
}

Vec3 Vec3Mult(Vec3 a, Vec3 b) {
    Vec3 res = {a.x * b.x, a.y * b.y, a.z * b.z};
    return res;
}

Vec3 ComputeNormal(Vec3 *v0, Vec3 *v1, Vec3 *v2) {
    Vec3 edge1 = Vec3Sub(*v1, *v0);
    Vec3 edge2 = Vec3Sub(*v2, *v0);

    Vec3 normal = Vec3Cross(edge1, edge2);
    return Vec3Normalize(normal);
}
