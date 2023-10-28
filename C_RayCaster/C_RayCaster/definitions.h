#pragma once
#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <stdint.h>
#include <math.h>

#define PI 3.14159265
#define TWO_PI 6.28318530

#define TILE_SIZE 64
#define NUM_TEXTURES 8

#define MINIMAP_SCALE_FACTOR 0.2

#define WINDOW_WIDTH 1280 // Before: 800
#define WINDOW_HEIGHT 800 // Before: 600

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
#define INTENSITY_FACTOR 0.7

#define FPS 30
#define FRAME_TIME_LENGTH (1000 / FPS)

#endif // DEFINITIONS_H