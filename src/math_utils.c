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
                       {s,0,c}}};
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

Vec3 MultiplyMat3Vec3(Matrice3x3 mat, Vec3 vec) {
    Vec3 ret = {mat.M[0][0] * vec.x + mat.M[0][1] * vec.y + mat.M[0][2] * vec.z,
                mat.M[1][0] * vec.x + mat.M[1][1] * vec.y + mat.M[1][2] * vec.z,
                mat.M[2][0] * vec.x + mat.M[2][1] * vec.y + mat.M[2][2] * vec.z};
    return ret;
}

float Vec3DotProduct(Vec3 a, Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

