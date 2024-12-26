#ifndef INPUT_H
#define INPUT_H
#include "camera.h"
#include <SDL2/SDL_video.h>

void doInput(Camera *cam, SDL_Window *window); 
void lockMouse(SDL_Window *window); 

#endif
