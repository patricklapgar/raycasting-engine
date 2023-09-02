#pragma once
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <math.h>

#define FALSE 0
#define TRUE 1

#define PI 3.14159265
#define TWO_PI 6.28318530

#define TILE_SIZE 64
#define MAP_ROWS 13
#define MAP_COLS 20
#define NUM_TEXTURES 8

#define MINIMAP_SCALE_FACTOR 0.2

#define WINDOW_WIDTH (MAP_COLS * TILE_SIZE)
#define WINDOW_HEIGHT (MAP_ROWS * TILE_SIZE)

#define FOV_ANGLE (60 * (PI / 180))

#define NUM_RAYS WINDOW_WIDTH
#define DIST_PROJ_PLANE ((WINDOW_WIDTH / 2) / tan(FOV_ANGLE / 2))

/* 
Format of colors:
	0xFF FF FF FF
	  a  r  g  b
*/
#define CEILING_COLOR 0xFF333333
#define FLOOR_COLOR 0xFF777777

#define FPS 30
#define FRAME_TIME_LENGTH (1000 / FPS)

#endif