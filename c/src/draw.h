#ifndef _DRAW_H_
#define _DRAW_H_

#include <SDL2/SDL_render.h>

void drawRect(SDL_Renderer *renderer, int x, int y, int w, int h, int r, int g, int b, int a);
void drawLine(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, int r, int g, int b, int a);
void drawLineF(SDL_Renderer *renderer, float x1, float y1, float x2, float y2, int r, int g, int b,
               int a);
#endif  // _DRAW_H_
