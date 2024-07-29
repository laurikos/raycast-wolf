#include "draw.h"

#include <SDL2/SDL_render.h>
#include <stdio.h>

#include "defs.h"

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

// --------------------------------------------------------------------------------------------
// Draw to frame buffer:

void setPixel(uint32_t *frameBuffer, int x, int y, uint32_t color) {
    frameBuffer[y * SCREEN_WIDTH + x] = color;
}

void frameBufferDrawRect(uint32_t *frameBuffer, int x, int y, int w, int h, uint32_t color) {
    for (int i = x; i < x + w; i++) {
        for (int j = y; j < y + h; j++) {
            if (i < 0 || i >= SCREEN_WIDTH || j < 0 || j >= SCREEN_HEIGHT) {
                printf("drawRect() out of bounds (x: %d, y:%d)\n", i, j);
                continue;
            }

            setPixel(frameBuffer, i, j, color);
        }
    }
}

void frameBufferDrawLine(uint32_t *frameBuffer, int x1, int y1, int x2, int y2, uint32_t color) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;

    int err = (dx > dy ? dx : -dy) / 2;
    int e2;

    for (;;) {
        setPixel(frameBuffer, x1, y1, color);

        if (x1 == x2 && y1 == y2) {
            break;
        }

        e2 = err;

        if (e2 > -dx) {
            err -= dy;
            x1 += sx;
        }

        if (e2 < dy) {
            err += dx;
            y1 += sy;
        }
    }
}

// --------------------------------------------------------------------------------------------

void changeColorIntensity(uint32_t *color, float intensity) {
    // notice that our pixel format is SDL_PIXELFORMAT_ABGR8888

    uint8_t a = (*color & 0xFF000000) >> 24;
    uint8_t b = (*color & 0x00FF0000) >> 16;
    uint8_t g = (*color & 0x0000FF00) >> 8;
    uint8_t r = (*color & 0x000000FF);

    r = (uint8_t)(r * intensity);
    g = (uint8_t)(g * intensity);
    b = (uint8_t)(b * intensity);

    *color = (a << 24) | (b << 16) | (g << 8) | r;
}
