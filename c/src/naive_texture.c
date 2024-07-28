#include "naive_texture.h"

#include <stdio.h>

#include "premade_textures.h"

NaiveTexture *NaiveTexture_new(int width, int height, uint32_t *data) {
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
    const uint32_t wallcolor = 0xAB8211FF;
    const uint32_t black = 0x000000FF;

    uint32_t *data = malloc(sizeof(uint32_t) * width * height);
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

    textures->data[0] = (uint32_t *)REDBRICK_TEXTURE;
    textures->data[1] = (uint32_t *)PURPLESTONE_TEXTURE;
    textures->data[2] = (uint32_t *)MOSSYSTONE_TEXTURE;
    textures->data[3] = (uint32_t *)GRAYSTONE_TEXTURE;
    textures->data[4] = (uint32_t *)COLORSTONE_TEXTURE;
    textures->data[5] = (uint32_t *)BLUESTONE_TEXTURE;
    textures->data[6] = (uint32_t *)WOOD_TEXTURE;
    textures->data[7] = (uint32_t *)EAGLE_TEXTURE;

    return textures;
}

void PremadeTextures_free(PremadeTextures *textures) { free(textures); }
