#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <SDL2/SDL_pixels.h>
#include <stdint.h>

typedef struct {
    int width;
    int height;
    uint32_t* data;
    SDL_PixelFormat* format;
} Texture;

Texture* Texture_new(const char* file);
void Texture_free(Texture* self);

// -------------------------------------------------------------------------------------------

typedef struct {
    Texture* textures[8];
} TextureManager;

TextureManager* TextureManager_new();
void TextureManager_free(TextureManager* self);

#endif  // _TEXTURE_H_
