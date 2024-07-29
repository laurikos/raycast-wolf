#ifndef _MAP_H_
#define _MAP_H_

#include <SDL2/SDL_render.h>

void drawMap(SDL_Renderer *renderer);
int getMapTileAtIndex(int row, int col);
int getMapTileAtCoords(float x, float y);
void drawMinimap(uint32_t *frameBuffer, int mapX, int mapY, int mapWidth, int mapHeight);
#endif  // _MAP_H_
