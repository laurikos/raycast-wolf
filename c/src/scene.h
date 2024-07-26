#ifndef _SCENE_H_
#define _SCENE_H_

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>

#include "app.h"
#include "player.h"
#include "ray.h"

typedef struct {
    Player *player;
    Rays *rays;
    Uint32 *textureBuffer;
    SDL_Texture *texture;
} Scene;

Scene *prepareScene(SDL_Renderer *renderer);
void destroyScene(Scene *scene);

void drawScene(Scene *scene, SDL_Renderer *renderer);

void updateScene(Scene *scene, App *app);

#endif  // _SCENE_H_
