#include "textures.h"
#include <stdio.h>

static const char* textureFilenames[NUM_TEXTURES] = {
	"redbrick.png",
	"purplestone.png",
	"mossystone.png",
	"graystone.png",
	"colorstone.png",
	"bluestone.png",
	"wood.png",
	"eagle.png"
};


void loadWallTextures(void) {
	for (int i = 0; i < NUM_TEXTURES; i++) {
		upng_t* upng;
		upng = upng_new_from_file(textureFilenames[i]);
		if (upng != NULL) {
			upng_decode(upng);
			
			if (upng_get_error(upng) == UPNG_EOK) {
				wallTextures[i].upngTexture = upng;
				wallTextures[i].width = upng_get_width(upng);
				wallTextures[i].height = upng_get_height(upng);
				wallTextures[i].texture_buffer = (uint32_t*)upng_get_buffer(upng);
			}
		} else {
			fprintf(stderr, "Couldn't find .png file.\n");
		}
	}
}

void freeWallTextures(void) {
	for (int i = 0; i < NUM_TEXTURES; i++) {
		upng_free(wallTextures[i].upngTexture);
	}
}