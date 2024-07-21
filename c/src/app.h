#ifndef _APP_H_
#define _APP_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_keyboard.h>

typedef struct {
    int isRunning;
    SDL_Window *window;
    SDL_Renderer *renderer;
    int sdlInitialized;
    double deltaTime;
    struct {
        int x;
        int y;
        int buttonPressed;
    } mouse;
    struct {
        int turnDirection;
        int moveDirection;
    } player;
} App;

App *App_new();
void App_free(App *self);
void App_initSDL(App *self);

#endif  // _APP_H_
