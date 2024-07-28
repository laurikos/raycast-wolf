#ifndef _NAIVE_TEXTURE_H_
#define _NAIVE_TEXTURE_H_

#include <stdint.h>

typedef struct {
    uint32_t *data[8];
} PremadeTextures;

PremadeTextures *PremadeTextures_new();
void PremadeTextures_free(PremadeTextures *textures);

#endif  // _NAIVE_TEXTURE_H_
