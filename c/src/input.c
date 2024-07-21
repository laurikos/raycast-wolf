#include "input.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>

// TODO: finish this function properly
void processInput(App *app) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                app->isRunning = 0;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    app->isRunning = 0;
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
