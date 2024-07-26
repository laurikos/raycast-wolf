#include "player.h"

#include <stdlib.h>

#include "defs.h"
#include "draw.h"
#include "map.h"

Player* createPlayer(float x, float y, float w, float h, float rotationSpeed, float moveSpeed) {
    Player* player = malloc(sizeof(Player));

    player->x = x;
    player->y = y;
    player->w = w;
    player->h = h;
    player->rotation = PI / 2;
    player->rotationSpeed = rotationSpeed;
    player->moveSpeed = moveSpeed;
    player->turnDirection = 0;
    player->moveDirection = 0;

    return player;
}

void playerFree(Player* player) { free(player); }

// drawPlayer is a function that draws the player on the minimap.
void drawPlayer(Player* player, SDL_Renderer* renderer) {
    // Rendering starts from the top left corner coordinates of the player
    // so let's center this by shifting the coordinates by half of the player's width and height
    const float playerCenterX = player->x - player->w / 2;
    const float playerCenterY = player->y - player->h / 2;

    drawRect(renderer, playerCenterX * MINIMAP_SCALE_FACTOR, playerCenterY * MINIMAP_SCALE_FACTOR,
             player->w * MINIMAP_SCALE_FACTOR, player->h * MINIMAP_SCALE_FACTOR, 20, 40, 190, 255);

    // The purpose of this line was just to draw a line from the player to the direction it is
    // facing. now that we draw the rays, we don't need this line anymore... (maybe ? for debugging
    // purposes) drawLineF(renderer, player->x * MINIMAP_SCALE_FACTOR, player->y *
    // MINIMAP_SCALE_FACTOR,
    //           (player->x + cos(player->rotation) * 40) * MINIMAP_SCALE_FACTOR,
    //           (player->y + sin(player->rotation) * 40) * MINIMAP_SCALE_FACTOR, 180, 40, 5, 255);
}

void updatePlayer(Player* player, int turnDirection, int moveDirection, float deltaTime) {
    player->turnDirection = turnDirection;
    player->moveDirection = moveDirection;

    player->rotation += player->turnDirection * player->rotationSpeed * deltaTime;

    float moveStep = player->moveDirection * player->moveSpeed * deltaTime;

    float newPlayerX = player->x + cos(player->rotation) * moveStep;
    float newPlayerY = player->y + sin(player->rotation) * moveStep;

    if (!getMapTileAtCoords(newPlayerX, newPlayerY)) {
        player->x = newPlayerX;
        player->y = newPlayerY;
    }
}
