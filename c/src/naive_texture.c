#include "naive_texture.h"

#include <stdio.h>

#include "premade_textures.h"

NaiveTexture *NaiveTexture_new(int width, int height, Uint32 *data) {
    NaiveTexture *texture = malloc(sizeof(NaiveTexture));
    texture->width = width;
    texture->height = height;
    texture->data = data;
    return texture;
}

void NaiveTexture_free(NaiveTexture *texture) {
    free(texture->data);
    free(texture);
}

NaiveTexture *NaiveTexture_newWallTexture(int width, int height) {
    const Uint32 wallcolor = 0xAB8211FF;
    const Uint32 black = 0x000000FF;

    Uint32 *data = malloc(sizeof(Uint32) * width * height);
    if (!data) {
        fprintf(stderr, "NaiveTexture_newWallTexture() malloc failed\n");
        return NULL;
    }

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            int idx = y * width + x;

            if ((y % 8 == 0) || (x % 8 == 0)) {
                data[idx] = black;
            } else {
                data[idx] = wallcolor;
            }
        }
    }

    return NaiveTexture_new(width, height, data);
}

// PremadeTextures_new() loads premade uint8 texture arrays into a PremadeTextures struct
// These textures are defined in "premade_textures.h"
PremadeTextures *PremadeTextures_new() {
    PremadeTextures *textures = malloc(sizeof(PremadeTextures));
    if (!textures) {
        fprintf(stderr, "PremadeTextures_get() malloc failed\n");
        return NULL;
    }

    textures->data[0] = (Uint32 *)REDBRICK_TEXTURE;
    textures->data[1] = (Uint32 *)PURPLESTONE_TEXTURE;
    textures->data[2] = (Uint32 *)MOSSYSTONE_TEXTURE;
    textures->data[3] = (Uint32 *)GRAYSTONE_TEXTURE;
    textures->data[4] = (Uint32 *)COLORSTONE_TEXTURE;
    textures->data[5] = (Uint32 *)BLUESTONE_TEXTURE;
    textures->data[6] = (Uint32 *)WOOD_TEXTURE;
    textures->data[7] = (Uint32 *)EAGLE_TEXTURE;

    return textures;
}

void PremadeTextures_free(PremadeTextures *textures) { free(textures); }
