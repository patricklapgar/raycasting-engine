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
	"eagle.png",
	"barrel.png",
	"table.png",
	"guard.png",
	"light.png",
	"armor.png"
};


void loadTextures(void) {
	for (int i = 0; i < NUM_TEXTURES; i++) {
		upng_t* upng = upng_new_from_file(textureFilenames[i]);
		if (upng != NULL) {
			upng_decode(upng);
			
			if (upng_get_error(upng) == UPNG_EOK)
				textures[i] = upng;
			else
				printf("Error decoding texture %s\n", textureFilenames[i]);
		} else {
			fprintf(stderr, "Error loading texture %s\n", textureFilenames[i]);
		}
	}
}

void freeTextures(void) {
	for (int i = 0; i < NUM_TEXTURES; i++) {
		upng_free(textures[i]);
	}
}