#include "app.h"

#include <SDL2/SDL_blendmode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdlib.h>

#include "defs.h"

App *App_new() {
    App *self = malloc(sizeof(App));
    if (self == NULL) {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }
    self->renderer = NULL;
    self->window = NULL;
    self->sdlInitialized = 0;
    return self;
}

void App_free(App *self) {
    if (self->renderer != NULL) {
        SDL_DestroyRenderer(self->renderer);
    }
    if (self->window != NULL) {
        SDL_DestroyWindow(self->window);
    }
    if (self->sdlInitialized) {
        SDL_Quit();
    }

    free(self);
}

void App_initSDL(App *self) {
    int rendererFlags, windowFlags;

    rendererFlags = SDL_RENDERER_ACCELERATED;

    windowFlags = 0;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        exit(1);
    }

    self->sdlInitialized = 1;

    self->window = SDL_CreateWindow("Raycast", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                    SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);
    if (self->window == NULL) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    self->renderer = SDL_CreateRenderer(self->window, -1, rendererFlags);
    if (self->renderer == NULL) {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(self->window);
        SDL_Quit();
        exit(1);
    }

    SDL_SetRenderDrawBlendMode(self->renderer, SDL_BLENDMODE_BLEND);
}
