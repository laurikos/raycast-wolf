#ifndef _NAIVE_TEXTURE_H_
#define _NAIVE_TEXTURE_H_

#include <SDL2/SDL_stdinc.h>

typedef struct {
    int width;
    int height;
    uint32_t *data;
} NaiveTexture;

NaiveTexture *NaiveTexture_newWallTexture(int width, int height);
void NaiveTexture_free(NaiveTexture *texture);

typedef struct {
    uint32_t *data[8];
} PremadeTextures;

PremadeTextures *PremadeTextures_new();
void PremadeTextures_free(PremadeTextures *textures);

#endif  // _NAIVE_TEXTURE_H_
