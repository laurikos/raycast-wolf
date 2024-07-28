#ifndef _SCENE_H_
#define _SCENE_H_

#include <SDL2/SDL_render.h>
#include <stdint.h>

#include "app.h"
#include "naive_texture.h"
#include "player.h"
#include "ray.h"

typedef struct {
    Player *player;
    Rays *rays;
    uint32_t *textureBuffer;
    SDL_Texture *texture;
    PremadeTextures *textures;
} Scene;

Scene *prepareScene(SDL_Renderer *renderer);
void destroyScene(Scene *scene);

void drawScene(Scene *scene, SDL_Renderer *renderer);

void updateScene(Scene *scene, App *app);

#endif  // _SCENE_H_
