#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>

char doInput(void) {
    SDL_Event event;
    char key;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                exit(0);
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        exit(0);
                        break;
                    case SDLK_w:
                        key = 'w';
                        break;
                    case SDLK_a:
                        key = 'a';
                        break;
                    case SDLK_s:
                        key = 's';
                        break;
                    case SDLK_d:
                        key = 'd';
                        break;
                    case SDLK_e:
                        key = '^';
                        break;
                    case SDLK_q:
                        key = '_';
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }

    return key;
}
