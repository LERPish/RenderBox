#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include "defs.h"
#include "structs.h"
#include "camera.h"

extern unsigned int framebuffer[SCREEN_HEIGHT][SCREEN_WIDTH];
extern float zbuffer[SCREEN_HEIGHT][SCREEN_WIDTH];

void RenderFramebuffer(App *app);
void ClearFramebuffer(unsigned int color);
int DrawPixel(int x, int y, float z, unsigned int color);
void line(Vec2 v0, Vec2 v1, unsigned int color);
int ComputeBarycentric(Vec2 v0, Vec2 v1, Vec2 v2, int x, int y, float *lambda1, float *lambda2, float *lambda3);
void RasterizeTriangle (Vec3 vv0, Vec3 vv1, Vec3 vv2, Camera cam, unsigned int color);

#endif
