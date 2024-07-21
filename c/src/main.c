#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>

#include "app.h"
#include "defs.h"
#include "draw.h"
#include "input.h"

static int playerPosX = 0;
static int playerPosY = 0;

void setup(App *app) {
    playerPosX = 0;
    playerPosY = 0;
}

void logic(App *app) {
    playerPosX = playerPosX + (2 * app->deltaTime);
    playerPosY = playerPosY + (2 * app->deltaTime);
}

void update(App *app) {
    double tmpDeltaTime;

    while (app->deltaTime > 1) {
        tmpDeltaTime = app->deltaTime;

        app->deltaTime = 1;

        logic(app);

        app->deltaTime = (tmpDeltaTime - 1);
    }

    logic(app);
}

void render(App *app) {
    SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
    SDL_RenderClear(app->renderer);

    drawRect(app, playerPosX, playerPosY, 32, 32, 255, 0, 0, 255);

    SDL_RenderPresent(app->renderer);
}

int main(int argc, char *argv[]) {
    App *app = App_new();
    App_initSDL(app);

    setup(app);

    long frameStart;
    long frameTime;

    // bascially 16ms per frame with 60 FPS
    const long frameDelay = 1000 / TARGET_FPS;

    while (app->isRunning) {
        frameStart = SDL_GetTicks();

        processInput(app);

        update(app);

        render(app);

        frameTime = SDL_GetTicks() - frameStart;

        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
        // ---------------------------
        // SDL_Delay(1);

        app->deltaTime = UPDATE_RATE * (SDL_GetTicks() - frameStart);
    }

    App_free(app);

    return 0;
}
