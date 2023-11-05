#pragma once
#ifndef SPRITE_H
#define SPRITE_H

#include "graphics.h"
#include "player.h"

typedef struct {
	float x;
	float y;
	float distance;
	float angle;
	int texture;
}sprite_t;

void renderSpriteProjection(void);
void renderMapSprites(void);

#endif // SPRITE_H
