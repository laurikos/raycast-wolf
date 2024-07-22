#ifndef _RAY_H_
#define _RAY_H_

#include <SDL2/SDL_render.h>
#include <stdlib.h>

#include "consts.h"

typedef enum { RAY_HORIZONTAL, RAY_VERTICAL } RayIntercept;

typedef struct {
    float angle;
    float hitX;
    float hitY;
    float distance;
    int hit;
    int hitVertical;
    RayDirection direction;
} Ray;

typedef struct {
    int hit;
    float distance;
    float hitX;
    float hitY;
} RayResult;

void setRayDirection(Ray *ray);
void castRay(Ray *ray, int stripID, float playerX, float playerY, RayIntercept interceptType,
             RayResult *result);

typedef struct {
    Ray *rays;
    int numRays;
} Rays;

Rays *Rays_init();
void Rays_free(Rays *self);

void doRays(Rays *self, float playerX, float playerY, float playerRotation);
void drawRays(Rays *self, SDL_Renderer *renderer, float playerX, float playerY);

#endif  // _RAY_H_
