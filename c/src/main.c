#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>

#include "app.h"
#include "defs.h"
#include "input.h"
#include "logic.h"
#include "scene.h"

void update(App *app, Scene *scene) {
    double tmpDeltaTime;

    while (app->deltaTime > 1) {
        tmpDeltaTime = app->deltaTime;

        app->deltaTime = 1;

        doLogic(app, scene);

        app->deltaTime = (tmpDeltaTime - 1);
    }

    doLogic(app, scene);
}

void render(App *app, Scene *scene) {
    SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
    SDL_RenderClear(app->renderer);

    // drawRect(app, playerPosX, playerPosY, 32, 32, 255, 0, 0, 255);
    drawScene(scene, app->renderer);

    SDL_RenderPresent(app->renderer);
}

int main(int argc, char *argv[]) {
    App *app = App_new();
    App_initSDL(app);

    // Setup the game
    Scene *scene = prepareScene();
    if (!scene) {
        App_free(app);
        return 1;
    }

    long frameStart;
    long frameTime;

    // bascially 16ms per frame with 60 FPS
    const long frameDelay = 1000 / TARGET_FPS;

    while (app->isRunning) {
        frameStart = SDL_GetTicks();

        processInput(app);

        update(app, scene);

        render(app, scene);

        frameTime = SDL_GetTicks() - frameStart;

        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
        // ---------------------------
        // SDL_Delay(1);

        app->deltaTime = UPDATE_RATE * (SDL_GetTicks() - frameStart);
    }

    destroyScene(scene);
    App_free(app);

    return 0;
}
