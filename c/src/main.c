#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>

#include "app.h"

static int isRunning = 1;

int playerPosX = 0;
int playerPosY = 0;

void drawRect(App *app, int x, int y, int w, int h, int r, int g, int b, int a) {
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;

    SDL_SetRenderDrawBlendMode(app->renderer, a < 255 ? SDL_BLENDMODE_BLEND : SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(app->renderer, r, g, b, a);
    SDL_RenderFillRect(app->renderer, &rect);
}

void setup(App *app) {
    // doSetup();
    playerPosX = 0;
    playerPosY = 0;
}

// TODO: finish this function properly
void processInput(App *app) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                isRunning = 0;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    isRunning = 0;
                }
                break;

                // case SDL_KEYDOWN:
                // 	doKeyDown(&event.key);
                // 	break;

                // case SDL_KEYUP:
                // 	doKeyUp(&event.key);
                // 	break;

                // case SDL_MOUSEBUTTONDOWN:
                // 	doMouseDown(&event.button);
                // 	break;

                // case SDL_MOUSEBUTTONUP:
                // 	doMouseUp(&event.button);
                // 	break;

                // case SDL_JOYBUTTONDOWN:
                // 	doButtonDown(&event.jbutton);
                // 	break;

                // case SDL_JOYBUTTONUP:
                // 	doButtonUp(&event.jbutton);
                // 	break;

                // case SDL_JOYAXISMOTION:
                // 	doJoyAxis(&event.jaxis);
                // 	break;

            default:
                break;
        }
    }

    // SDL_GetMouseState(&app.mouse.x, &app.mouse.y);
}

void update(App *app) {
    // doUpdate();
    playerPosX++;
    playerPosY++;
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

    while (isRunning) {
        processInput(app);
        update(app);
        render(app);

        SDL_Delay(16);
    }

    App_free(app);
    return 0;
}
