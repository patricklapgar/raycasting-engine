#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <float.h>
#include "definitions.h"
#include "graphics.h"
#include "map.h"

typedef struct {
	float x;
	float y;
	float width;
	float height;
	int turnDirection; // -1 for left, +1 for right
	int walkDirection; // -1 for back, +1 for front
	float rotationAngle;
	float walkSpeed;
	float turnSpeed;
} player_t;

// Global declaration of the player
extern player_t player;

void movePlayer(float deltaTime);
void renderPlayer(void);

#endif // PLAYER_H