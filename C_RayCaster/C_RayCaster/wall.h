#pragma once
#ifndef WALL_H
#define WALL_H

#include <math.h>
#include "definitions.h"
#include "ray.h"
#include "player.h"
#include "graphics.h"
#include "textures.h"

void changeColorIntensity(uint32_t* color, float factor);
void renderWallProjection(void);

#endif // WALL_H