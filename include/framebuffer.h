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

#endif
