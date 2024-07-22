#include "scene.h"

#include <SDL2/SDL_render.h>
#include <stdio.h>

#include "defs.h"
#include "map.h"
#include "player.h"
#include "ray.h"

Scene* prepareScene() {
    Scene* scene = malloc(sizeof(Scene));
    if (!scene) {
        fprintf(stderr, "prepareScene() malloc failed\n");
        return NULL;
    }

    Player* player =
        createPlayer((float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2, 20, 20, TO_RADIANS(3), 2);
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

    return scene;
}

void destroyScene(Scene* scene) {
    playerFree(scene->player);
    Rays_free(scene->rays);
    free(scene);
}

void drawScene(Scene* scene, SDL_Renderer* renderer) {
    drawMap(renderer);

    drawPlayer(scene->player, renderer);

    drawRays(scene->rays, renderer, scene->player->x, scene->player->y);
}

void updateScene(Scene* scene, App* app) {
    updatePlayer(scene->player, app->player.turnDirection, app->player.moveDirection,
                 app->deltaTime);

    doRays(scene->rays, scene->player->x, scene->player->y, scene->player->rotation);
}
