#include "scene.h"

#include <SDL2/SDL_render.h>
#include <stdio.h>

#include "defs.h"
#include "map.h"
#include "player.h"

Scene* prepareScene() {
    Scene* scene = malloc(sizeof(Scene));
    if (!scene) {
        fprintf(stderr, "malloc failed\n");
        return NULL;
    }

    Player* player =
        createPlayer((float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2, 20, 20, TO_RADIANS(3), 2);
    if (!player) {
        free(scene);
        return NULL;
    }

    scene->player = player;

    return scene;
}

void destroyScene(Scene* scene) {
    playerFree(scene->player);
    free(scene);
}

void drawScene(Scene* scene, SDL_Renderer* renderer) {
    drawMap(renderer);

    drawPlayer(scene->player, renderer);
}

void updateScene(Scene* scene, App* app) {
    updatePlayer(scene->player, app->player.turnDirection, app->player.moveDirection,
                 app->deltaTime);
}
