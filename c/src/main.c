#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>

#include "app.h"

static int isRunning = 1;

void doInput(void) {
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

void doRender(App *app) {
    SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
    SDL_RenderClear(app->renderer);
    SDL_RenderPresent(app->renderer);
}

int main(int argc, char *argv[]) {
    App *app = App_new();
    App_initSDL(app);

    while (1) {
        doInput();
        // doUpdate();
        doRender(app);
        if (!isRunning) {
            break;
        }

        SDL_Delay(16);
    }

    App_free(app);
    return 0;
}
