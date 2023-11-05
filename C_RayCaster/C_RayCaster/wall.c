#include "wall.h"

void changeColorIntensity(uint32_t* color, float factor) {
	uint32_t a = (*color & 0xFF000000);
	uint32_t r = (*color & 0x00FF0000) * factor;
	uint32_t g = (*color & 0x0000FF00) * factor;
	uint32_t b = (*color & 0x000000FF) * factor;
	*color = a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF);
}

void renderWallProjection(void) {
	for (int x = 0; x < NUM_RAYS; x++) {
		// Calculate perpendicular distance to avoid fish-eye distortion
		float perpDistance = rays[x].distance * cos(rays[x].rayAngle - player.rotationAngle);
		float projectedWallHeight = (TILE_SIZE / perpDistance) * DIST_PROJ_PLANE;

		//int wallStripHeight = (int)projectedWallHeight;

		int wallTopY = (WINDOW_HEIGHT / 2) - (projectedWallHeight / 2);
		wallTopY = wallTopY < 0 ? 0 : wallTopY;

		int wallBottomY = (WINDOW_HEIGHT / 2) + (projectedWallHeight / 2);
		wallBottomY = wallBottomY > WINDOW_HEIGHT ? WINDOW_HEIGHT : wallBottomY;

		// set the color of the ceiling
		for (int y = 0; y < wallTopY; y++)
			drawPixel(x, y, CEILING_COLOR);

		int textureNum = rays[x].wallHitContent - 1;
		
		// Query texture width/height from upng library
		int textureWidth = upng_get_width(textures[textureNum]);
		int textureHeight = upng_get_height(textures[textureNum]);

		// calculate texture offset X
		int textureOffsetX;
		if (rays[x].wasHitVertical)
			textureOffsetX = (int)rays[x].wallHitY % textureHeight;
		else
			textureOffsetX = (int)rays[x].wallHitX % textureWidth;

		// render the wall from wallTopY to wallBottomY
		for (int y = wallTopY; y < wallBottomY; y++) {
			// calculate texture offset Y
			int distanceFromTop = y + (projectedWallHeight / 2) - (WINDOW_HEIGHT / 2);
			int textureOffsetY = distanceFromTop * ((float)textureHeight / projectedWallHeight);

			uint32_t* wallTextureBuffer = (uint32_t*)upng_get_buffer(textures[textureNum]);
			// set the color of the wall based on the color from the texture buffer
			uint32_t texelColor = wallTextureBuffer[(textureWidth * textureOffsetY) + textureOffsetX];
			
			// Make the pixel color darker if the ray hit was vertical
			if (rays[x].wasHitVertical) changeColorIntensity(&texelColor, INTENSITY_FACTOR);

			drawPixel(x, y, texelColor);
		}

		// set the color of the floor
		for (int y = wallBottomY; y < WINDOW_HEIGHT; y++)
			drawPixel(x, y, FLOOR_COLOR);
	}
}