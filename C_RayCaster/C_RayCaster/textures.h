#ifndef TEXTURES_H
#define TEXTURES_H

#include <stdint.h>
#include "definitions.h"
#include "upng.h"

typedef struct {
	int width;
	int height;
	upng_t* upngTexture;
	uint32_t* texture_buffer;
} texture_t;

texture_t wallTextures[NUM_TEXTURES];

void loadWallTextures(void);
void freeWallTextures(void);

#endif // TEXTURES_H