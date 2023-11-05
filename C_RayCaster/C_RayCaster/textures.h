#ifndef TEXTURES_H
#define TEXTURES_H

#include <stdint.h>
#include "definitions.h"
#include "upng.h"

/*
typedef struct {
	int width;
	int height;
	upng_t* upngTexture;
	uint32_t* texture_buffer;
} texture_t;
*/

upng_t* textures[NUM_TEXTURES];

void loadTextures(void);
void freeTextures(void);

#endif // TEXTURES_H