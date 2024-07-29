#include "ray.h"

#include <limits.h>
#include <math.h>

#include "consts.h"
#include "defs.h"
#include "draw.h"
#include "map.h"
#include "stdio.h"

static float normalizeAngle(float angle) {
    float normalizedAngle = remainderf(angle, (2 * PI));

    if (normalizedAngle < 0) {
        normalizedAngle = (2 * PI) + normalizedAngle;
    }

    return normalizedAngle;
}

static float distanceBetweenPoints(float x1, float y1, float x2, float y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

void setRayDirection(Ray *ray) {
    // And just a reminder; because our "Map" is a 2D array of integers
    // where growing row number means going down and growing column number
    // means going right, we are thinking the angles clockwise
    // Which at least for me is a bit counter intuitive since I've always learned
    // the unit circles and angles in counter clockwise direction..

    // So if the angle is between 0 and PI (0 and 180 degrees) then the ray is facing down
    // if the angle is between PI and 2*PI (180 and 360 degrees) then the ray is facing up
    // if the anlgle is between PI/2 and 3*PI/2 (90 and 270 degrees) then the ray is facing left
    // if the angle is between 3*PI/2 and 2*PI or 0 and PI/2 (270 and 360 or 0 and 90 degrees)
    // then the ray is facing right

    ray->direction = 0;

    if (ray->angle > 0 && ray->angle < PI) {
        ray->direction |= FACING_DOWN;
    } else {
        ray->direction |= FACING_UP;
    }

    if (ray->angle < PI / 2 || ray->angle > 3 * PI / 2) {
        ray->direction |= FACING_RIGHT;
    } else {
        ray->direction |= FACING_LEFT;
    }
}

void castRay(Ray *ray, int stripID, float playerX, float playerY, RayIntercept interceptType,
             RayResult *result) {
    (void)stripID;
    float xIntercept, yIntercept;
    float xStep, yStep;

    int verticalHit = 0;
    int horizontalHit = 0;

    float verticalHitX = 0;
    float verticalHitY = 0;

    float horizontalHitX = 0;
    float horizontalHitY = 0;

    if (interceptType == RAY_HORIZONTAL) {
        // Compute initial values for horizontal casting
        yIntercept = floor(playerY / TILE_SIZE) * TILE_SIZE;
        yIntercept += (ray->direction & FACING_DOWN) ? TILE_SIZE : 0;
        xIntercept = playerX + (yIntercept - playerY) / tan(ray->angle);

        yStep = TILE_SIZE;
        if (ray->direction & FACING_UP) {
            yStep *= -1;
        }
        xStep = TILE_SIZE / tan(ray->angle);
        if ((ray->direction & FACING_LEFT) && xStep > 0) {
            xStep *= -1;
        }
        if ((ray->direction & FACING_RIGHT) && xStep < 0) {
            xStep *= -1;
        }

    } else {
        // Compute initial values for vertical casting
        xIntercept = floor(playerX / TILE_SIZE) * TILE_SIZE;
        xIntercept += (ray->direction & FACING_RIGHT) ? TILE_SIZE : 0;
        yIntercept = playerY + (xIntercept - playerX) * tan(ray->angle);

        xStep = TILE_SIZE;
        if (ray->direction & FACING_LEFT) {
            xStep *= -1;
        }

        yStep = TILE_SIZE * tan(ray->angle);
        if ((ray->direction & FACING_UP) && yStep > 0) {
            yStep *= -1;
        }
        if ((ray->direction & FACING_DOWN) && yStep < 0) {
            yStep *= -1;
        }
    }

    float nextHorizontalTileX = xIntercept;
    float nextHorizontalTileY = yIntercept;

    float nextVerticalTileX = xIntercept;
    float nextVerticalTileY = yIntercept;

    if (interceptType == RAY_HORIZONTAL) {
        while (nextHorizontalTileX >= 0 && nextHorizontalTileX <= MAP_NUM_COLS * TILE_SIZE &&
               nextHorizontalTileY >= 0 && nextHorizontalTileY <= MAP_NUM_ROWS * TILE_SIZE) {
            float xToCheck = nextHorizontalTileX;
            // with horizontal intercept I have to check the y coordinate inside the cell
            // rather than the y coordinate of the grid line. So if we are facing up
            // we minus one (pixel) from the y coordinate of the grid line
            // and this is again because we the negative numbers are going up
            // because the map is int[][] and the y coordinate is converted to row number.
            float yToCheck = nextHorizontalTileY;
            if (ray->direction & FACING_UP) {
                yToCheck -= 1;
            }

            int tileContent = getMapTileAtCoords(xToCheck, yToCheck);
            if (tileContent > 0) {
                horizontalHitX = nextHorizontalTileX;
                horizontalHitY = nextHorizontalTileY;
                horizontalHit = tileContent;
                break;
            } else {
                nextHorizontalTileX += xStep;
                nextHorizontalTileY += yStep;
            }
        }
    } else {
        while (nextVerticalTileX >= 0 && nextVerticalTileX <= MAP_NUM_COLS * TILE_SIZE &&
               nextVerticalTileY >= 0 && nextVerticalTileY <= MAP_NUM_ROWS * TILE_SIZE) {
            float xToCheck = nextVerticalTileX;
            if (ray->direction & FACING_LEFT) {
                xToCheck -= 1;
            }
            float yToCheck = nextVerticalTileY;

            int tileContent = getMapTileAtCoords(xToCheck, yToCheck);
            if (tileContent > 0) {
                verticalHitX = nextVerticalTileX;
                verticalHitY = nextVerticalTileY;
                verticalHit = tileContent;
                break;
            } else {
                nextVerticalTileX += xStep;
                nextVerticalTileY += yStep;
            }
        }
    }

    float horizontalHitDistance = 0.0f;
    float verticalHitDistance = 0.0f;

    if (interceptType == RAY_HORIZONTAL) {
        horizontalHitDistance =
            distanceBetweenPoints(playerX, playerY, horizontalHitX, horizontalHitY);
        result->distance = horizontalHitDistance;
        result->hit = horizontalHit;
        result->hitX = horizontalHitX;
        result->hitY = horizontalHitY;
    } else if (interceptType == RAY_VERTICAL) {
        verticalHitDistance = distanceBetweenPoints(playerX, playerY, verticalHitX, verticalHitY);
        result->distance = verticalHitDistance;
        result->hit = verticalHit;
        result->hitX = verticalHitX;
        result->hitY = verticalHitY;
    } else {
        printf("[castRay]: Unknown intercept type\n");
    }
}

Rays *Rays_init() {
    Rays *self = malloc(sizeof(Rays));
    if (self == NULL) {
        fprintf(stderr, "malloc failed\n");
        return NULL;
    }
    self->numRays = NUM_RAYS;
    self->rays = malloc(sizeof(Ray) * self->numRays);
    if (self->rays == NULL) {
        fprintf(stderr, "malloc failed\n");
        return NULL;
    }

    for (int i = 0; i < self->numRays; i++) {
        self->rays[i].angle = 0;
        self->rays[i].hitX = 0;
        self->rays[i].hitY = 0;
        self->rays[i].distance = 0;
        self->rays[i].hit = 0;
        self->rays[i].hitVertical = 0;
        self->rays[i].direction = 0;
    }

    return self;
}

void Rays_free(Rays *self) {
    free(self->rays);

    free(self);
}

void doRays(Rays *self, float playerX, float playerY, float playerRotation) {
    float rayAngle = playerRotation - (FOV_ANGLE / 2);
    const float distanceProjectionPlane = (SCREEN_WIDTH / 2.0f) / tanf(FOV_ANGLE / 2.0f);

    int stripID;

    RayResult horizontalResult;
    RayResult verticalResult;

    for (stripID = 0; stripID < self->numRays; stripID++) {
        Ray *ray = &self->rays[stripID];

        rayAngle = playerRotation + atan((stripID - (float)NUM_RAYS / 2) / distanceProjectionPlane);

        ray->angle = normalizeAngle(rayAngle);

        setRayDirection(ray);

        if ((ray->direction & FACING_DOWN) && ray->direction & FACING_UP) {
            printf("[doRays__WARN]: ray %d, facing down and up. angle: %.2f (in_deg: %.2f)\n",
                   stripID, ray->angle, TO_DEGREES(ray->angle));
        }
        if ((ray->direction & FACING_LEFT) && ray->direction & FACING_RIGHT) {
            printf("[doRays__WARN]: ray %d, facing left and right. angle: %.2f (in_deg: %.2f)\n",
                   stripID, ray->angle, TO_DEGREES(ray->angle));
        }

        castRay(ray, stripID, playerX, playerY, RAY_HORIZONTAL, &horizontalResult);
        castRay(ray, stripID, playerX, playerY, RAY_VERTICAL, &verticalResult);

        if (!horizontalResult.hit) {
            horizontalResult.distance = FLT_MAX;
        }
        if (!verticalResult.hit) {
            verticalResult.distance = FLT_MAX;
        }

        if (horizontalResult.distance < verticalResult.distance) {
            ray->hit = horizontalResult.hit;
            ray->hitX = horizontalResult.hitX;
            ray->hitY = horizontalResult.hitY;
            ray->distance = horizontalResult.distance;
            ray->hitVertical = 0;
        } else {
            ray->hit = verticalResult.hit;
            ray->hitX = verticalResult.hitX;
            ray->hitY = verticalResult.hitY;
            ray->distance = verticalResult.distance;
            ray->hitVertical = 1;
        }

        if (ray->hit == 0) {
            printf(
                "[doRays__WARN_NO_HIT]: ray %d, hit: %d, hitX: %.2f, hitY: %.2f, distance: %.2f, "
                "is_vertical: %d, angle: %.2f (%.2f), facing: %d\n",
                stripID, ray->hit, ray->hitX, ray->hitY, ray->distance, ray->hitVertical,
                ray->angle, TO_DEGREES(ray->angle), ray->direction);
        }

        // rayAngle += FOV_ANGLE / NUM_RAYS;
    }
}

void drawRays(Rays *self, SDL_Renderer *renderer, float playerX, float playerY) {
    for (int i = 0; i < self->numRays; i++) {
        Ray *ray = &self->rays[i];

        drawLineF(renderer, playerX * MINIMAP_SCALE_FACTOR, playerY * MINIMAP_SCALE_FACTOR,
                  ray->hitX * MINIMAP_SCALE_FACTOR, ray->hitY * MINIMAP_SCALE_FACTOR, 10, 89, 98,
                  255);
    }
}
