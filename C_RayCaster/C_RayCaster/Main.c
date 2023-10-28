#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <float.h>
#include <SDL.h>
#include "definitions.h"
#include "graphics.h"
#include "map.h"
#include "ray.h"
#include "player.h"
#include "textures.h"
#include "wall.h"

bool isGameRunning = false;
int ticksLastFrame;

/*
void setup() {
	loadWallTextures();
}
*/

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

void render() {
	clearColorBuffer(0xFF000000);

	renderWallProjection();

	renderMap();
	renderRays();
	renderPlayer();

	renderColorBuffer();
}

void releaseResources(void) {
	freeWallTextures();
	destroyWindow();
}

int main() {
	isGameRunning = initializeWindow();

	//setup();
	loadWallTextures();

	while (isGameRunning) {
		processInput();
		update();
		render();
	}

	releaseResources();

	return 0;
}
