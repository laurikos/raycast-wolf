#ifndef _MAP_H_
#define _MAP_H_

#include <SDL2/SDL_render.h>

void drawMap(SDL_Renderer *renderer);
int getMapTileAtIndex(int row, int col);
int getMapTileAtCoords(float x, float y);
void drawMinimap();

#endif  // _MAP_H_
