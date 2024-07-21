#include "draw.h"

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
