#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include <float.h>
#include <SDL.h>
#include "definitions.h"
#include "graphics.h"
#include "map.h"
#include "ray.h"
#include "player.h"
#include "textures.h"

bool isGameRunning = false;
int ticksLastFrame;


void setup() {

	// Load all png texture files via upng.c
	loadWallTextures();
}

void processInput() {
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type) {
	case SDL_QUIT: {
		isGameRunning = false;
		break;
	}
	case SDL_KEYDOWN: {
		if (event.key.keysym.sym == SDLK_ESCAPE)
			isGameRunning = false;
		if (event.key.keysym.sym == SDLK_UP)
			player.walkDirection = +1;
		if (event.key.keysym.sym == SDLK_DOWN)
			player.walkDirection = -1;
		if (event.key.keysym.sym == SDLK_RIGHT)
			player.turnDirection = +1;
		if (event.key.keysym.sym == SDLK_LEFT)
			player.turnDirection = -1;
		break;
	}
	case SDL_KEYUP: {
		if (event.key.keysym.sym == SDLK_UP)
			player.walkDirection = 0;
		if (event.key.keysym.sym == SDLK_DOWN)
			player.walkDirection = 0;
		if (event.key.keysym.sym == SDLK_RIGHT)
			player.turnDirection = 0;
		if (event.key.keysym.sym == SDLK_LEFT)
			player.turnDirection = 0;
		break;
	}
	}
}

void update() {
	// waste some time until we reach the target frame time length
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticksLastFrame + FRAME_TIME_LENGTH));

	float deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;

	ticksLastFrame = SDL_GetTicks();

	movePlayer(deltaTime);
	castAllRays();
}

void renderWallProjection(void) {
	for (int x = 0; x < NUM_RAYS; x++) {
		float perpDistance = rays[x].distance * cos(rays[x].rayAngle - player.rotationAngle);
		float projectedWallHeight = (TILE_SIZE / perpDistance) * DIST_PROJ_PLANE;

		int wallStripHeight = (int)projectedWallHeight;

		int wallTopPixel = (WINDOW_HEIGHT / 2) - (wallStripHeight / 2);
		wallTopPixel = wallTopPixel < 0 ? 0 : wallTopPixel;

		int wallBottomPixel = (WINDOW_HEIGHT / 2) + (wallStripHeight / 2);
		wallBottomPixel = wallBottomPixel > WINDOW_HEIGHT ? WINDOW_HEIGHT : wallBottomPixel;

		// set the color of the ceiling
		for (int y = 0; y < wallTopPixel; y++)
			drawPixel(x, y, CEILING_COLOR);

		int textureNum = rays[x].wallHitContent - 1;
		int texture_width = wallTextures[textureNum].width;
		int texture_height = wallTextures[textureNum].height;

		// calculate texture offset X
		int textureOffsetX;
		if (rays[x].wasHitVertical)
			textureOffsetX = (int)rays[x].wallHitY % texture_height;
		else
			textureOffsetX = (int)rays[x].wallHitX % texture_width;

		// render the wall from wallTopPixel to wallBottomPixel
		for (int y = wallTopPixel; y < wallBottomPixel; y++) {
			// calculate texture offset Y
			int distanceFromTop = y + (wallStripHeight / 2) - (WINDOW_HEIGHT / 2);
			int textureOffsetY = distanceFromTop * ((float)texture_height / wallStripHeight);

			// set the color of the wall based on the color from the texture
			uint32_t texelColor = wallTextures[textureNum].texture_buffer[(texture_width * textureOffsetY) + textureOffsetX];
			drawPixel(x, y, texelColor);
		}

		// set the color of the floor
		for (int y = wallBottomPixel; y < WINDOW_HEIGHT; y++)
			drawPixel(x, y, FLOOR_COLOR);
	}
}

void render() {
	clearColorBuffer(0xFF000000);

	renderWallProjection();

	renderMap();
	//renderRays();
	renderPlayer();

	renderColorBuffer();
}

void releaseResources(void) {
	freeWallTextures();
	destroyWindow();
}

int main() {
	isGameRunning = initializeWindow();

	setup();

	while (isGameRunning) {
		processInput();
		update();
		render();
	}

	releaseResources();

	return 0;
}
