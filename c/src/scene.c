#include "scene.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <complex.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "defs.h"
#include "draw.h"
#include "map.h"
#include "player.h"
#include "ray.h"
#include "texture.h"

Scene* prepareScene(SDL_Renderer* renderer) {
    Scene* scene = malloc(sizeof(Scene));
    if (!scene) {
        fprintf(stderr, "prepareScene() malloc failed\n");
        return NULL;
    }

    Player* player =
        createPlayer((float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2, 20, 20, TO_RADIANS(1), 2);
    if (!player) {
        fprintf(stderr, "prepareScene::createPlayer() failed\n");
        free(scene);
        return NULL;
    }

    scene->player = player;

    Rays* rays = Rays_init();
    if (!rays) {
        fprintf(stderr, "prepareScene::Rays_init() failed\n");
        playerFree(player);
        free(scene);
        return NULL;
    }

    scene->rays = rays;

    uint32_t* textureBuffer = malloc(sizeof(uint32_t) * SCREEN_WIDTH * SCREEN_HEIGHT);
    if (!textureBuffer) {
        fprintf(stderr, "prepareScene() malloc texture buffer failed\n");
        playerFree(player);
        Rays_free(rays);
        free(scene);
        return NULL;
    }

    scene->textureBuffer = textureBuffer;

    scene->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888,
                                       SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!scene->texture) {
        const char* error = SDL_GetError();
        fprintf(stderr, "SDL_CreateTexture() failed: %s\n", error);
        playerFree(player);
        Rays_free(rays);
        free(textureBuffer);
        free(scene);
        return NULL;
    }

    TextureManager* textures = TextureManager_new();
    if (!textures) {
        fprintf(stderr, "prepareScene::TextureManager_new() failed\n");
        playerFree(player);
        Rays_free(rays);
        free(textureBuffer);
        SDL_DestroyTexture(scene->texture);
        free(scene);
        return NULL;
    }
    scene->textures = textures;

    return scene;
}

void destroyScene(Scene* scene) {
    playerFree(scene->player);
    Rays_free(scene->rays);
    free(scene->textureBuffer);
    TextureManager_free(scene->textures);
    SDL_DestroyTexture(scene->texture);
    free(scene);
}

void clearTextureBuffer(uint32_t* const textureBuffer, const uint32_t color) {
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        textureBuffer[i] = color;

        // If I wanna test something ...
        // int x = i % SCREEN_WIDTH;
        // int y = i / SCREEN_HEIGHT;
    }
}

void drawTextureBuffer(SDL_Renderer* renderer, SDL_Texture* texture, uint32_t* textureBuffer) {
    // SDL Documents that this will work but is a slow operation
    // that docs say is good for rendering static textures
    // https://wiki.libsdl.org/SDL2/SDL_UpdateTexture
    // ---------------------------------------------------------------------------------------
    // int res = SDL_UpdateTexture(texture, NULL, textureBuffer, SCREEN_WIDTH * sizeof(uint32_t));
    // if (res != 0) {
    //     fprintf(stderr, "drawTextureBuffer() failed: %s\n", SDL_GetError());
    // }

    // res = SDL_RenderCopy(renderer, texture, NULL, NULL);
    // if (res != 0) {
    //     fprintf(stderr, "drawTextureBuffer() failed: %s\n", SDL_GetError());
    // }

    // ---------------------------------------------------------------------------------------
    // This is the SDL2 documented way to update a texture
    // that is changing rapidly

    void* pixels;
    int pitch;

    SDL_LockTexture(texture, NULL, &pixels, &pitch);
    memcpy(pixels, textureBuffer, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(uint32_t));
    SDL_UnlockTexture(texture);

    SDL_RenderCopy(renderer, texture, NULL, NULL);
}

void generateWallProjection(Scene* scene) {
    const uint32_t ceilingColor = 0xFFEDC798;
    const uint32_t floorColor = 0xFF534B54;

    int x;
    for (x = 0; x < scene->rays->numRays; x++) {
        Ray* ray = &scene->rays->rays[x];

        // correct the fish eye effect
        const float correctedAngle = ray->angle - scene->player->rotation;
        const float correctedDistance = ray->distance * cosf(correctedAngle);

        // get the distance and the height of the wall to be projected
        const float distanceProjectionPlane = (SCREEN_WIDTH / 2.0f) / tanf(FOV_ANGLE / 2);
        const float projectedWallHeight = (TILE_SIZE / correctedDistance) * distanceProjectionPlane;

        const int wallStripHeight = (int)projectedWallHeight;
        int renderWallStartY = (SCREEN_HEIGHT / 2) - (wallStripHeight / 2);
        renderWallStartY = renderWallStartY < 0 ? 0 : renderWallStartY;

        int renderWallEndY = (SCREEN_HEIGHT / 2) + (wallStripHeight / 2);
        renderWallEndY = renderWallEndY > SCREEN_HEIGHT ? SCREEN_HEIGHT : renderWallEndY;

        // get properties for getting the texture for each pixel:
        int wallTextureOffsetY = 0;
        int wallTextureOffsetX = 0;

        const int textureIdx = ray->hit - 1;
        const Texture* texture = scene->textures->textures[textureIdx];
        const uint32_t* textureData = scene->textures->textures[textureIdx]->data;

        if (ray->hitVertical) {
            wallTextureOffsetX = (int)ray->hitY % TILE_SIZE;
        } else {
            wallTextureOffsetX = (int)ray->hitX % TILE_SIZE;
        }

        // do the drawing:
        for (int y = renderWallStartY; y < renderWallEndY; y++) {
            const int distanceFromTop = y + (wallStripHeight / 2) - (SCREEN_HEIGHT / 2);

            wallTextureOffsetY = distanceFromTop * ((float)texture->height / wallStripHeight);

            const int textureLocation = wallTextureOffsetY * texture->width + wallTextureOffsetX;
            if (textureLocation < 0 || textureLocation >= texture->width * texture->height) {
                printf("Texture location out of bounds: %d (tex_idx: %d)\n", textureLocation,
                       textureIdx);
                continue;
            }

            uint32_t wallTextureColor = textureData[textureLocation];
            // and change the color based on the distance:
            // intensity should be between 0.0 and 1.0
            // lets make it porportional to the distance:
            int maxDistance =
                (MAP_NUM_COLS > MAP_NUM_ROWS ? MAP_NUM_COLS : MAP_NUM_ROWS) * TILE_SIZE;
            // add just some buffer to max distance - otherwise the walls will be too dark
            maxDistance += 100;
            float intensity = 1.0f - (correctedDistance / maxDistance);
            changeColorIntensity(&wallTextureColor, intensity);

            scene->textureBuffer[y * SCREEN_WIDTH + x] = wallTextureColor;
        }

        // ceiling:
        for (int y = 0; y < renderWallStartY; y++) {
            scene->textureBuffer[y * SCREEN_WIDTH + x] = ceilingColor;
        }

        // floor:
        for (int y = renderWallEndY; y < SCREEN_HEIGHT; y++) {
            scene->textureBuffer[y * SCREEN_WIDTH + x] = floorColor;
        }
    }
}

void drawScene(Scene* scene, SDL_Renderer* renderer) {
    const uint32_t clearColor = 0xFF000000;

    // Initialize the texture buffer with a clear color
    clearTextureBuffer(scene->textureBuffer, clearColor);

    // ---------------------------------------------------------------------------------------

    // generate the wall projection
    // this sets the texture buffer with the wall projection
    // computed from the rays casted from the player
    generateWallProjection(scene);

    // ---------------------------------------------------------------------------------------
    // Draw the minimap:

    // for now the textureBuffer is actually only function arg that gets used...
    drawMinimap(scene->textureBuffer, 0, 0, 300, 300);

    drawPlayerOnMinimap(scene->player, scene->textureBuffer);

    drawRaysFB(scene->rays, scene->textureBuffer, 0, 0, scene->player->x, scene->player->y);

    // ---------------------------------------------------------------------------------------

    // And finally draw the texture buffer to the screen:
    drawTextureBuffer(renderer, scene->texture, scene->textureBuffer);
}

void updateScene(Scene* scene, App* app) {
    updatePlayer(scene->player, app->player.turnDirection, app->player.moveDirection,
                 app->deltaTime);

    doRays(scene->rays, scene->player->x, scene->player->y, scene->player->rotation);
}
