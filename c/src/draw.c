#include "draw.h"

#include <SDL2/SDL_render.h>

void drawRect(SDL_Renderer *renderer, int x, int y, int w, int h, int r, int g, int b, int a) {
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;

    SDL_SetRenderDrawBlendMode(renderer, a < 255 ? SDL_BLENDMODE_BLEND : SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderFillRect(renderer, &rect);
}

void drawLine(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, int r, int g, int b, int a) {
    SDL_SetRenderDrawBlendMode(renderer, a < 255 ? SDL_BLENDMODE_BLEND : SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void drawLineF(SDL_Renderer *renderer, float x1, float y1, float x2, float y2, int r, int g, int b,
               int a) {
    SDL_SetRenderDrawBlendMode(renderer, a < 255 ? SDL_BLENDMODE_BLEND : SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderDrawLineF(renderer, x1, y1, x2, y2);
}
