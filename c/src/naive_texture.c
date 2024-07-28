#include "naive_texture.h"

#include <stdio.h>
#include <stdlib.h>

#include "premade_textures.h"

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
