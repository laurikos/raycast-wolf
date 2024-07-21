#ifndef DEFS_H
#define DEFS_H

// #define SCREEN_WIDTH 1600
// #define SCREEN_HEIGHT 900

#define TARGET_FPS 60
#define UPDATE_RATE (TARGET_FPS * 1.0 / 1000)

#define PI 3.14159265358979323846
#define TO_RADIANS(angleDegrees) (angleDegrees * PI / 180.0)
#define TO_DEGREES(angleRadians) (angleRadians * 180.0 / PI)

#define TILE_SIZE 64
#define MAP_NUM_ROWS 13
#define MAP_NUM_COLS 20

#define SCREEN_HEIGHT (MAP_NUM_ROWS * TILE_SIZE)
#define SCREEN_WIDTH (MAP_NUM_COLS * TILE_SIZE)

// #define MINIMAP_SCALE_FACTOR 0.3
#define MINIMAP_SCALE_FACTOR 1.0

#define FOV_ANGLE (60 * (PI / 180))
#define NUM_RAYS SCREEN_WIDTH

#endif  // DEFS_H_
