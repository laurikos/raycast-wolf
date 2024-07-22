#include "logic.h"

#include "scene.h"

void doLogic(App *app, Scene *scene) {
    // updatePlayer(scene->player, app->player.turnDirection, app->player.moveDirection,
    //              app->deltaTime);

    updateScene(scene, app);
}
