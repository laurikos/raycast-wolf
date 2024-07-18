#ifndef _APP_H_
#define _APP_H_

#include <SDL2/SDL.h>

typedef struct {
        SDL_Window *window;
        SDL_Renderer *renderer;
        int sdlInitialized;
} App;

App *App_new();
void App_free(App *self);
void App_initSDL(App *self);

#endif  // _APP_H_
