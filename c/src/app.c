#include "app.h"

#include <SDL2/SDL_blendmode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdlib.h>

App *App_new() {
    App *self = malloc(sizeof(App));
    if (self == NULL) {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }
    self->renderer = NULL;
    self->window = NULL;
    self->sdlInitialized = 0;
    self->deltaTime = 1.0;
    self->isRunning = 1;
    self->mouse.x = 0;
    self->mouse.y = 0;
    self->mouse.buttonPressed = 0;
    self->player.turnDirection = 0;
    self->player.moveDirection = 0;

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

    static int display_in_use = 0; /* Only using first display */

    int i, display_mode_count;
    SDL_DisplayMode mode;
    Uint32 f;

    SDL_Log("SDL_GetNumVideoDisplays(): %i", SDL_GetNumVideoDisplays());

    display_mode_count = SDL_GetNumDisplayModes(display_in_use);
    if (display_mode_count < 1) {
        SDL_Log("SDL_GetNumDisplayModes failed: %s", SDL_GetError());
        return;
    }
    SDL_Log("SDL_GetNumDisplayModes: %i", display_mode_count);

    for (i = 0; i < display_mode_count; ++i) {
        if (SDL_GetDisplayMode(display_in_use, i, &mode) != 0) {
            SDL_Log("SDL_GetDisplayMode failed: %s", SDL_GetError());
            return;
        }
        f = mode.format;

        SDL_Log("Mode %i\tbpp %i\t%s\t%i x %i", i, SDL_BITSPERPIXEL(f), SDL_GetPixelFormatName(f),
                mode.w, mode.h);
    }

    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(display_in_use, &dm);
    SDL_Log("Current display mode: %s %d x %d", SDL_GetPixelFormatName(dm.format), dm.w, dm.h);

    int screenWidth = dm.w;
    int screenHeight = dm.h;

    self->window = SDL_CreateWindow("Raycast", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                    screenWidth, screenHeight, windowFlags);
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
