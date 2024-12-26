#include "math_utils.h"
#include <math.h>

Matrice3x3 RxAxis(float theta) {
    float c = cosf(theta);
    float s = sinf(theta);
    Matrice3x3 ret = {{{1,0,0},
                       {0,c,s},
                       {0,-s,c}}};
    return ret;
}

Matrice3x3 RyAxis(float theta) {
    float c = cosf(theta);
    float s = sinf(theta);
    Matrice3x3 ret = {{{c,0,-s},
                       {0,1,0},
                       {s,0,c}}};
    return ret;
}

Matrice3x3 RzAxis(float theta) {
    float c = cosf(theta);
    float s = sinf(theta);
    Matrice3x3 ret = {{{c,s,0},
                       {-s,c,0},
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

Vec4 MultiplyMat4Vec4(Matrice4x4 mat, Vec4 vec) {
    Vec4 ret = {mat.M[0][0] * vec.x + mat.M[0][1] * vec.y + mat.M[0][2] * vec.z + mat.M[0][3] * vec.w,
                mat.M[1][0] * vec.x + mat.M[1][1] * vec.y + mat.M[1][2] * vec.z + mat.M[1][3] * vec.w,
                mat.M[2][0] * vec.x + mat.M[2][1] * vec.y + mat.M[2][2] * vec.z + mat.M[2][3] * vec.w,
                mat.M[3][0] * vec.x + mat.M[3][1] * vec.y + mat.M[3][2] * vec.z + mat.M[3][3] * vec.w};
    return ret;
}

Matrice4x4 MatQuickInverse(Matrice4x4 m) {
    Matrice4x4 mat;
    mat.M[0][0] = m.M[0][0];mat.M[0][1] = m.M[1][0];mat.M[0][2] = m.M[2][0];
    mat.M[1][0] = m.M[0][1];mat.M[1][1] = m.M[1][1];mat.M[1][2] = m.M[2][1];
    mat.M[2][0] = m.M[0][2];mat.M[2][1] = m.M[1][2];mat.M[2][2] = m.M[2][2];
    mat.M[3][0] = 0.0f;mat.M[3][1] = 0.0f;mat.M[3][2] = 0.0f;

    mat.M[0][3] = -(m.M[0][3] * mat.M[0][0] + m.M[1][3] * mat.M[0][1] + m.M[2][3] * mat.M[0][2]);
    mat.M[1][3] = -(m.M[0][3] * mat.M[1][0] + m.M[1][3] * mat.M[1][1] + m.M[2][3] * mat.M[1][2]);
    mat.M[2][3] = -(m.M[0][3] * mat.M[2][0] + m.M[1][3] * mat.M[2][1] + m.M[2][3] * mat.M[2][2]);

    mat.M[3][3] = 1.0f; 

    return mat;
}


Matrice4x4 MatPointAt(Vec3 pos, Vec3 target, Vec3 up) {
    Vec3 newForward = Vec3Sub(target, pos);
    newForward = Vec3Normalize(newForward);

    Vec3 newRight = Vec3Cross(up, newForward);
    newRight = Vec3Normalize(newRight);

    Vec3 newUp = Vec3Cross(newForward, newRight);

    Matrice4x4 mat = {.M = {
        { newRight.x, newUp.x, -newForward.x, pos.x },
        { newRight.y, newUp.y, -newForward.y, pos.y },
        { newRight.z, newUp.z, -newForward.z, pos.z },
        { 0.0f, 0.0f, 0.0f, 1.0f }
    }};

    return mat;
}

Matrice4x4 MatTranspose(Matrice4x4 mat) {
    for (int x = 0; x < 4; x++) {
        for (int y = x; y < 4; y++) {
            if (x == y) {
                continue;
            }
            float a = mat.M[x][y];
            mat.M[x][y] = mat.M[y][x];
            mat.M[y][x] = a;
        }
    }

    return mat;
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

Vec2 Vec2Sub(Vec2 a, Vec2 b) {
    Vec2 ret = (Vec2){ a.x - b.x, a.y - b.y };
    return ret;
}

float Vec2DotProduct(Vec2 a, Vec2 b) {
    return a.x * b.x + a.y * b.y;
}

void RGBToHex(Color *color) {
    unsigned int red = (unsigned int)(color->r * 255.0f) & 0xFF;
    unsigned int green = (unsigned int)(color->g * 255.0f) & 0xFF;
    unsigned int blue = (unsigned int)(color->b * 255.0f) & 0xFF;
    unsigned int alpha = (unsigned int)(color->a * 255.0f) & 0xFF;

    unsigned int hex = (alpha << 24) | (red << 16) | (green << 8) | blue;

    color->hex = hex;
}

void HexToRGB(Color *color) {
    unsigned int hex = color->hex;
    color->r = ((hex >> 16) & 0xFF) / 255.0f;
    color->g = ((hex >> 8) & 0xFF) / 255.0f;
    color->b = (hex & 0xFF) / 255.0f;
    color->a = ((hex >> 24) & 0xFF) / 255.0f;
}

Color CreateColorRGB(float r, float g, float b, float a) {
    Color color;
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;
    RGBToHex(&color);
    return color;
}

Color CreateColorHex(unsigned int hex) {
    Color color;
    color.hex = hex;
    HexToRGB(&color);
    return color;
}

