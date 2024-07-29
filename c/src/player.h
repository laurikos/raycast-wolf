#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <SDL2/SDL_render.h>

typedef struct {
    float x;
    float y;
    float w;
    float h;

    // turn direction; -1 for left, 1 for right, 0 otherwise
    int turnDirection;

    // move direction; -1 for backwards, 1 forwards, 0 otherwise
    int moveDirection;

    float rotation;
    float rotationSpeed;

    float moveSpeed;
} Player;

Player* createPlayer(float x, float y, float w, float h, float rotationSpeed, float moveSpeed);
void playerFree(Player* player);
void drawPlayer(Player* player, SDL_Renderer* renderer);
void drawPlayerOnMinimap(Player* player, uint32_t* textureBuffer);
void updatePlayer(Player* player, int turnDirection, int moveDirection, float deltaTime);

#endif  // _PLAYER_H_
