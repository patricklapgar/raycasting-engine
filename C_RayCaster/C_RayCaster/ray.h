#pragma once
#ifndef RAY_H
#define RAY_H

#include <stdbool.h>
#include <limits.h>
#include "definitions.h"
#include "player.h"
#include "utils.h"
//#include <float.h>

typedef struct {
	float rayAngle;
	float wallHitX;
	float wallHitY;
	float distance;
	bool wasHitVertical;
	int wallHitContent;
} ray_t;

// Global declaration of an array of rays
extern ray_t rays[NUM_RAYS];

void castRay(float rayAngle, int stripId);
void castAllRays(void);
void renderMapRays(void);


#endif // RAY_H
