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

// "720p" : 	1280 × 720 maybe?
// #define SCREEN_HEIGHT 720
// #define SCREEN_WIDTH 1280

// Window is created by SDL_GetCurrentDisplayMode
// but the pixels drawn are 1920x1080
#define SCREEN_HEIGHT 1080
#define SCREEN_WIDTH 1920

#define MINIMAP_SCALE_FACTOR 0.3

#define FOV_ANGLE (60 * (PI / 180))
#define NUM_RAYS SCREEN_WIDTH

#endif  // DEFS_H_
