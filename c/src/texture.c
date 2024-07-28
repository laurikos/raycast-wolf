#include "texture.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_surface.h>
#include <complex.h>
#include <stdio.h>

const char* textureFilePathsRel[8] = {"../images/redbrick.png",   "../images/purplestone.png",
                                      "../images/graystone.png",  "../images/bluestone.png",
                                      "../images/mossystone.png", "../images/wood.png",
                                      "../images/colorstone.png", "../images/eagle.png"};

Texture* Texture_new(const char* file) {
    Texture* texture = malloc(sizeof(Texture));
    if (!texture) {
        fprintf(stderr, "Texture_new() malloc failed\n");
        return NULL;
    }

    SDL_Surface* surface = IMG_Load(file);
    if (!surface) {
        fprintf(stderr, "Texture_new() IMG_Load failed: %s\n", IMG_GetError());
        free(texture);
        return NULL;
    }
    SDL_PixelFormat* pixelFormat = surface->format;
    texture->format = pixelFormat;

    int width = surface->w;
    int height = surface->h;
    texture->width = width;
    texture->height = height;

    texture->data = malloc(sizeof(uint32_t) * width * height);
    if (!texture->data) {
        fprintf(stderr, "Texture_new() malloc failed\n");
        SDL_FreeSurface(surface);
        free(texture);
        return NULL;
    }

    if (surface->pixels == NULL) {
        fprintf(stderr, "Texture_new() surface->pixels is NULL\n");
        SDL_FreeSurface(surface);
        free(texture->data);
        free(texture);
        return NULL;
    }

    SDL_LockSurface(surface);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            texture->data[y * width + x] = ((uint32_t*)surface->pixels)[y * width + x];
        }
    }

    const char* formatName = SDL_GetPixelFormatName(pixelFormat->format);
    printf("loaded texture %s: { \"width\": %d, \"height\": %d, \"format\": %s }\n", file, width,
           height, formatName);

    SDL_UnlockSurface(surface);

    SDL_FreeSurface(surface);

    return texture;
}

void Texture_free(Texture* self) {
    free(self->data);
    free(self);
}

// -------------------------------------------------------------------------------------------

TextureManager* TextureManager_new() {
    TextureManager* self = malloc(sizeof(TextureManager));
    if (!self) {
        fprintf(stderr, "TextureManager_new() malloc failed\n");
        return NULL;
    }

    for (int i = 0; i < 8; i++) {
        self->textures[i] = Texture_new(textureFilePathsRel[i]);
        if (!self->textures[i]) {
            fprintf(stderr, "TextureManager_new() Texture_new() failed\n");
            for (int j = 0; j < i; j++) {
                Texture_free(self->textures[j]);
            }
            free(self);
            return NULL;
        }
    }

    return self;
}
void TextureManager_free(TextureManager* self) {
    for (int i = 0; i < 8; i++) {
        Texture_free(self->textures[i]);
    }
    free(self);
}
