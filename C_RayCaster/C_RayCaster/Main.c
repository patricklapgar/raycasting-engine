#include <stdio.h>
#include <SDL.h>
#include "constants.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int isGameRunning = FALSE;

int initWindow(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error initializing SDL.\n");
		return FALSE;
	}

	window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_BORDERLESS
	);

	if (!window) {
		fprintf(stderr, "Error creating SDL window.\n");
		return FALSE;
	}

	renderer = SDL_CreateRenderer(
		window,
		-1,
		0
	);

	if (!renderer) {
		fprintf(stderr, "Error creating SDL renderer.\n");
		return FALSE;
	}

	SDL_SetRenderDrawBlendMode(
		renderer,
		SDL_BLENDMODE_BLEND
	);

	return TRUE;
}

void destroyWindow(void) {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int main(int argc, char* args[]) {
	isGameRunning = initWindow();

	setup();

	while (isGameRunning) {
		processInput();
		update();
		render();
	}

	destroyWindow();

	return 0;
}