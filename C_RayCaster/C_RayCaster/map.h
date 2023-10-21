#pragma once
#ifndef MAP_H
#define MAP_H

#include <stdbool.h>
#include "definitions.h"
#include "graphics.h"

#define MAP_ROWS 13
#define MAP_COLS 20

bool mapHasWallAt(float x, float y);
bool isInsideMap(float x, float y);
void renderMap(void);
int getMapAt(int yPos, int xPos);

#endif // MAP_H
