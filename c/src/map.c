#include "map.h"

#include <stdio.h>

#include "defs.h"
#include "draw.h"

static const int map[MAP_NUM_ROWS][MAP_NUM_COLS] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 2, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 5},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 5},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 5},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 5, 5, 5, 5, 5}};

void drawMap(SDL_Renderer *renderer) {
    for (int y = 0; y < MAP_NUM_ROWS; y++) {
        for (int x = 0; x < MAP_NUM_COLS; x++) {
            int tileX = x * TILE_SIZE;
            int tileY = y * TILE_SIZE;

            // For now every map tile is either black or white
            int tileColor = getMapTileAtIndex(y, x) != 0 ? 255 : 0;

            drawRect(renderer, tileX * MINIMAP_SCALE_FACTOR, tileY * MINIMAP_SCALE_FACTOR,
                     TILE_SIZE * MINIMAP_SCALE_FACTOR, TILE_SIZE * MINIMAP_SCALE_FACTOR, tileColor,
                     tileColor, tileColor, 255);
        }
    }
}

int getMapTileAtIndex(int row, int col) {
    if (row >= 0 && row < MAP_NUM_ROWS && col >= 0 && col < MAP_NUM_COLS) {
        return map[row][col];
    }

    fprintf(stderr, "Invalid map indices: row %d, col %d\n", row, col);
    return 1;
}

// Check if the given coordinates are inside a wall tile
// The coordinates are in world space and they need to be converted to map space
// thats where the floor (x / TILE_SIZE) comes in
int getMapTileAtCoords(float x, float y) {
    return getMapTileAtIndex((int)floor(y / TILE_SIZE), (int)floor(x / TILE_SIZE));
}

void drawMinimap(uint32_t *frameBuffer, int mapX, int mapY, int mapWidth, int mapHeight) {
    (void)mapX;
    (void)mapY;
    (void)mapWidth;
    (void)mapHeight;

    for (int y = 0; y < MAP_NUM_ROWS; y++) {
        for (int x = 0; x < MAP_NUM_COLS; x++) {
            int tileX = x * TILE_SIZE;
            int tileY = y * TILE_SIZE;

            int tileColor = getMapTileAtIndex(y, x) != 0 ? 0xFFFFFFFF : 0x00000000;

            frameBufferDrawRect(frameBuffer, (int)floor(tileX * MINIMAP_SCALE_FACTOR),
                                (int)floor(tileY * MINIMAP_SCALE_FACTOR),
                                (int)floor((TILE_SIZE + 5) * MINIMAP_SCALE_FACTOR),
                                (int)floor((TILE_SIZE + 5) * MINIMAP_SCALE_FACTOR), tileColor);
        }
    }
}
