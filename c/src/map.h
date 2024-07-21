#ifndef _MAP_H_
#define _MAP_H_

#include <SDL2/SDL_render.h>

void drawMap(SDL_Renderer *renderer);
int getMapTileAt(int row, int col);
int isWall(float x, float y);

#endif  // _MAP_H_
