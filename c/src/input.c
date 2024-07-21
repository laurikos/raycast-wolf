#include "input.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>

#include "app.h"

void doKeyDown(SDL_KeyboardEvent *event, App *app) {
    if (event->keysym.sym == SDLK_ESCAPE) {
        app->isRunning = 0;
        return;
    }
    switch (event->keysym.sym) {
        case SDLK_UP:
            app->player.moveDirection = 1;
            break;
        case SDLK_DOWN:
            app->player.moveDirection = -1;
            break;
        case SDLK_LEFT:
            app->player.turnDirection = -1;
            break;
        case SDLK_RIGHT:
            app->player.turnDirection = 1;
            break;
        default:
            break;
    }
}

void doKeyUp(SDL_KeyboardEvent *event, App *app) {
    switch (event->keysym.sym) {
        case SDLK_UP:
            app->player.moveDirection = 0;
            break;
        case SDLK_DOWN:
            app->player.moveDirection = 0;
            break;
        case SDLK_LEFT:
            app->player.turnDirection = 0;
            break;
        case SDLK_RIGHT:
            app->player.turnDirection = 0;
            break;
        default:
            break;
    }
}

void doMouseDown(SDL_MouseButtonEvent *event, App *app) {
    switch (event->button) {
        case SDL_BUTTON_LEFT:
            app->mouse.buttonPressed = 1;
            break;
        case SDL_BUTTON_RIGHT:
            app->mouse.buttonPressed = 2;
            break;
        default:
            break;
    }
}

void doMouseUp(SDL_MouseButtonEvent *event, App *app) {
    switch (event->button) {
        case SDL_BUTTON_LEFT:
            app->mouse.buttonPressed = 0;
            break;
        case SDL_BUTTON_RIGHT:
            app->mouse.buttonPressed = 0;
            break;
        default:
            break;
    }
}

// TODO: finish this function properly
void processInput(App *app) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                app->isRunning = 0;
                break;

            case SDL_KEYDOWN:
                doKeyDown(&event.key, app);
                break;

            case SDL_KEYUP:
                doKeyUp(&event.key, app);
                break;

            case SDL_MOUSEBUTTONDOWN:
                doMouseDown(&event.button, app);
                break;

            case SDL_MOUSEBUTTONUP:
                doMouseUp(&event.button, app);
                break;

            default:
                break;
        }
    }

    SDL_GetMouseState(&(app->mouse.x), &(app->mouse.y));
}
