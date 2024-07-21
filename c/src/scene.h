#ifndef _SCENE_H_
#define _SCENE_H_

#include <SDL2/SDL_render.h>

#include "app.h"
#include "player.h"

typedef struct {
    Player* player;
} Scene;

Scene* prepareScene();
void destroyScene(Scene* scene);

void drawScene(Scene* scene, SDL_Renderer* renderer);

void updateScene(Scene* scene, App* app);

#endif  // _SCENE_H_
