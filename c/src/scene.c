#include "scene.h"

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <stdio.h>
#include <time.h>

#include "defs.h"
#include "map.h"
#include "player.h"
#include "ray.h"

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

    Uint32* textureBuffer = malloc(sizeof(Uint32) * SCREEN_WIDTH * SCREEN_HEIGHT);
    if (!textureBuffer) {
        fprintf(stderr, "prepareScene() malloc texture buffer failed\n");
        playerFree(player);
        Rays_free(rays);
        free(scene);
        return NULL;
    }

    scene->textureBuffer = textureBuffer;

    scene->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
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

    return scene;
}

void destroyScene(Scene* scene) {
    playerFree(scene->player);
    Rays_free(scene->rays);
    free(scene->textureBuffer);
    SDL_DestroyTexture(scene->texture);
    free(scene);
}

void clearTextureBuffer(Uint32* const textureBuffer, const Uint32 color) {
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        textureBuffer[i] = color;

        // If I wanna test something ...
        // int x = i % SCREEN_WIDTH;
        // int y = i / SCREEN_HEIGHT;

        // if (x == y) {
        //     textureBuffer[i] = 0xFF1333FF;
        // }
    }
}

void drawTextureBuffer(SDL_Renderer* renderer, SDL_Texture* texture, Uint32* textureBuffer) {
    // SDL Documents that this will work but is a slow operation
    // that docs say is good for rendering static textures
    // https://wiki.libsdl.org/SDL2/SDL_UpdateTexture
    // ---------------------------------------------------------------------------------------
    // int res = SDL_UpdateTexture(texture, NULL, textureBuffer, SCREEN_WIDTH * sizeof(Uint32));
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
    memcpy(pixels, textureBuffer, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
    SDL_UnlockTexture(texture);

    SDL_RenderCopy(renderer, texture, NULL, NULL);
}

void generateWallProjection(Scene* scene) {
    const Uint32 wallcolor = 0xAB8211FF;
    const Uint32 wallcolorShaded = 0x453508FF;
    const Uint32 ceilingColor = 0x98C7EDFF;
    const Uint32 floorColor = 0x534B54FF;

    int x;
    for (x = 0; x < scene->rays->numRays; x++) {
        Ray* ray = &scene->rays->rays[x];

        // correct the fish eye effect
        // const correctedAngle = ray.rayAngle - player.rotationAngle;
        // const correctedDistance = rayDistance * Math.cos(correctedAngle);

        const float correctedAngle = ray->angle - scene->player->rotation;
        const float correctedDistance = ray->distance * cosf(correctedAngle);

        // get the distance and the height of the wall to be projected
        // const distanceProjectionPlane = (WINDOW_WIDTH / 2) / Math.tan(FOV_ANGLE / 2);
        // const projectedWallHeight = (TILE_SIZE / correctedDistance) * distanceProjectionPlane;

        float distanceProjectionPlane = (SCREEN_WIDTH / 2.0f) / tanf(FOV_ANGLE / 2);
        float projectedWallHeight = (TILE_SIZE / correctedDistance) * distanceProjectionPlane;

        int wallStripHeight = (int)projectedWallHeight;
        int renderWallStartY = (SCREEN_HEIGHT / 2) - (wallStripHeight / 2);
        renderWallStartY = renderWallStartY < 0 ? 0 : renderWallStartY;

        int renderWallEndY = (SCREEN_HEIGHT / 2) + (wallStripHeight / 2);
        renderWallEndY = renderWallEndY > SCREEN_HEIGHT ? SCREEN_HEIGHT : renderWallEndY;

        for (int y = renderWallStartY; y < renderWallEndY; y++) {
            scene->textureBuffer[y * SCREEN_WIDTH + x] = wallcolor;
            if (ray->hitVertical) {
                scene->textureBuffer[y * SCREEN_WIDTH + x] = wallcolorShaded;
            }
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
    const Uint32 clearColor = 0x000000FF;

    clearTextureBuffer(scene->textureBuffer, clearColor);
    generateWallProjection(scene);
    drawTextureBuffer(renderer, scene->texture, scene->textureBuffer);

    // Draw the minimap:
    drawMap(renderer);
    drawPlayer(scene->player, renderer);
    drawRays(scene->rays, renderer, scene->player->x, scene->player->y);
}

void updateScene(Scene* scene, App* app) {
    updatePlayer(scene->player, app->player.turnDirection, app->player.moveDirection,
                 app->deltaTime);

    doRays(scene->rays, scene->player->x, scene->player->y, scene->player->rotation);
}
