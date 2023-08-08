#include <stdio.h>
#include <limits.h>
#include <float.h>
#include <SDL.h>
#include "constants.h"

const int map[MAP_ROWS][MAP_COLS] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

struct Player {
	float x;
	float y;
	float width;
	float height;
	int turnDirection; // -1 for left, +1 for right
	int walkDirection; // -1 for backwards, +1 for forwards
	float rotationAngle;
	float walkSpeed;
	float turnSpeed;
} player;

struct Ray {
	float rayAngle;
	float wallHitX;
	float wallHitY;
	float distance;
	int wasHitVertical;
	int wasHitHorizontal;
	int isRayFaceUp;
	int isRayFaceDown;
	int isRayFaceLeft;
	int isRayFaceRight;
	int wallContent;
} rays[NUM_RAYS];

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int isGameRunning = FALSE;

int ticksLastFrame;

Uint32* colorBuffer = NULL;
SDL_Texture* colorBufferTexture;

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
	free(colorBuffer);
	SDL_DestroyTexture(colorBufferTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void setup(void) {
	player.x = WINDOW_WIDTH / 2;
	player.y = WINDOW_HEIGHT / 2;
	player.width = 5;
	player.height = 5;
	player.turnDirection = 0;
	player.walkDirection = 0;
	player.rotationAngle = PI / 2.0;
	player.walkSpeed = 100; // pixels/sec
	player.turnSpeed = 70.0 * (PI / 180.0); // radians/sec 

	colorBuffer = (Uint32*) malloc(sizeof(Uint32) * (Uint32)WINDOW_WIDTH * (Uint32)WINDOW_HEIGHT);
	colorBufferTexture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		WINDOW_WIDTH,
		WINDOW_HEIGHT
	);
}

int isWall(float x, float y) {
	if (x < 0 || x > WINDOW_WIDTH || y < 0 || y > WINDOW_HEIGHT) {
		return TRUE;
	}

	int mapIndexX = floor(x / TILE_SIZE);
	int mapIndexY = floor(y / TILE_SIZE);
	return map[mapIndexY][mapIndexX] != 0;
}

void movePlayer(float deltaT) {
	player.rotationAngle += player.turnDirection * player.turnSpeed * deltaT;
	float moveStep = player.walkDirection * player.walkSpeed * deltaT;
	float newPlayerX = player.x + cos(player.rotationAngle) * moveStep;
	float newPlayerY = player.y + sin(player.rotationAngle) * moveStep;

	// Check for wall collisions
	if (!isWall(newPlayerX, newPlayerY)) {
		player.x = newPlayerX;
		player.y = newPlayerY;
	}
}

void renderPlayer(void) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_Rect playerRect = {
		player.x * MINIMAP_SCALE_FACTOR,
		player.y * MINIMAP_SCALE_FACTOR,
		player.width * MINIMAP_SCALE_FACTOR,
		player.height * MINIMAP_SCALE_FACTOR
	};
	SDL_RenderFillRect(renderer, &playerRect);
	SDL_RenderDrawLine(
		renderer,
		player.x * MINIMAP_SCALE_FACTOR,
		player.y * MINIMAP_SCALE_FACTOR,
		(player.x + cos(player.rotationAngle) * 40) * MINIMAP_SCALE_FACTOR,
		(player.y + sin(player.rotationAngle) * 40) * MINIMAP_SCALE_FACTOR
	);
}

float normalizeAngle(float angle) {
	angle = remainderf(angle, TWO_PI);
	if (angle < 0)
		angle = TWO_PI + angle;
	return angle;
}

float distanceBetweenPoints(float x1 ,float y1, float x2, float y2) {
	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

void castRay(float angle, int strip) {
	angle = normalizeAngle(angle);
	
	int isRayFacingDown = angle > 0 && angle < PI;
	int isRayFacingUp = !isRayFacingDown;
	int isRayFacingRight = angle < 0.5 * PI || angle > 1.5 * PI;
	int isRayFacingLeft = !isRayFacingRight;

	// Horizontal collision detection
	float xintercept, yintercept;
	float xstep, ystep;
	// Horizontal ray-grid intersection

	int foundHorizontalWall = FALSE;
	float horizontalWallHitX = 0;
	float horizontalWallHitY = 0;
	int horizontalWallContent = 0;

	// Find the y-coordinate of the closest horizontal grid intersection
	yintercept = floor(player.y / TILE_SIZE) * TILE_SIZE;
	yintercept += isRayFacingDown ? TILE_SIZE : 0;

	// Find the x-coordinate of the closest horizontal grid intersection
	xintercept = player.x + ((yintercept - player.y) / tan(angle));

	// Calculate increment for xstep and ystep
	ystep = TILE_SIZE;
	ystep *= isRayFacingUp ? -1 : 1;

	xstep = TILE_SIZE / tan(angle);
	xstep *= (isRayFacingLeft && xstep > 0) ? -1 : 1;
	xstep *= (isRayFacingRight && xstep < 0) ? -1 : 1;

	float nextHorizontalX = xintercept;
	float nextHorizontalY = yintercept;

	// Increment xstep and ystep until a wall is hit
	while (nextHorizontalX >= 0 && nextHorizontalX <= WINDOW_WIDTH && nextHorizontalY >= 0 && nextHorizontalY <= WINDOW_HEIGHT) {
		float xPixelToCheck = nextHorizontalX;
		float yPixelToCheck = nextHorizontalY + (isRayFacingUp ? -1 : 0);

		// Found a wall
		if (isWall(xPixelToCheck, yPixelToCheck)) {
			horizontalWallHitX = nextHorizontalX;
			horizontalWallHitY = nextHorizontalY;
			horizontalWallContent = map[(int)floor(yPixelToCheck / TILE_SIZE)][(int)floor(xPixelToCheck / TILE_SIZE)];
			foundHorizontalWall = TRUE;
			break;
		} else {
			nextHorizontalX += xstep;
			nextHorizontalY += ystep;
		}
	}

	// Vertical collision detection
	int foundVerticalWall = FALSE;
	float verticalWallHitX = 0;
	float verticalWallHitY = 0;
	int verticalWallContent = 0;

	// Find the x-coordinate of the closest vertical grid intersection
	xintercept = floor(player.x / TILE_SIZE) * TILE_SIZE;
	xintercept += isRayFacingRight ? TILE_SIZE : 0;

	// Find the y-coordinate of the closest vertical grid intersection
	yintercept = player.y + (xintercept - player.x) * tan(angle);

	// Calculate increment for xstep and ystep
	xstep = TILE_SIZE;
	xstep *= isRayFacingLeft ? -1 : 1;

	ystep = TILE_SIZE * tan(angle);
	ystep *= (isRayFacingUp && ystep > 0) ? -1 : 1;
	ystep *= (isRayFacingDown && ystep < 0) ? -1 : 1;

	float nextVerticalX = xintercept;
	float nextVerticalY = yintercept;

	// Increment xstep and ystep until a wall is hit
	while (nextVerticalX >= 0 && nextVerticalX <= WINDOW_WIDTH && nextVerticalY >= 0 && nextVerticalY <= WINDOW_HEIGHT) {
		float xPixelToCheck = nextVerticalX + (isRayFacingLeft ? -1 : 0);
		float yPixelToCheck = nextVerticalY;

		if (isWall(xPixelToCheck, yPixelToCheck)) {
			foundVerticalWall = TRUE;
			verticalWallHitX = nextVerticalX;
			verticalWallHitY = nextVerticalY;
			verticalWallContent = map[(int)floor(yPixelToCheck / TILE_SIZE)][(int)floor(xPixelToCheck / TILE_SIZE)];
			break;
		} else {
			nextVerticalX += xstep;
			nextVerticalY += ystep;
		}
	}

	// Calculate both horizontal and vertical distances and choose the smallest value
	float horizontalHitDistance = foundHorizontalWall
		? distanceBetweenPoints(player.x, player.y, horizontalWallHitX, horizontalWallHitY)
		: FLT_MAX;

	float verticalHitDistance = foundVerticalWall
		? distanceBetweenPoints(player.x, player.y, verticalWallHitX, verticalWallHitY)
		: FLT_MAX;

	if (verticalHitDistance < horizontalHitDistance) {
		rays[strip].distance = verticalHitDistance;
		rays[strip].wallHitX = verticalWallHitX;
		rays[strip].wallHitY = verticalWallHitY;
		rays[strip].wallContent = verticalWallContent;
		rays[strip].wasHitVertical = TRUE;
	} else {
		rays[strip].distance = horizontalHitDistance;
		rays[strip].wallHitX = horizontalWallHitX;
		rays[strip].wallHitY = horizontalWallHitY;
		rays[strip].wallContent = horizontalWallContent;
		rays[strip].wasHitHorizontal = TRUE;
	}

	rays[strip].rayAngle = angle;
	rays[strip].isRayFaceDown = isRayFacingDown;
	rays[strip].isRayFaceUp = isRayFacingUp;
	rays[strip].isRayFaceLeft = isRayFacingLeft;
	rays[strip].isRayFaceRight = isRayFacingRight;
}

void castAllRays(void) {
	float rayAngle = player.rotationAngle - (FOV_ANGLE / 2);
	for (int rayStrip = 0; rayStrip < NUM_RAYS; rayStrip++) {
		castRay(rayAngle, rayStrip);
		rayAngle += FOV_ANGLE / NUM_RAYS;
	}
}

void renderMap(void) {
	for (int i = 0; i < MAP_ROWS; i++) {
		for (int j = 0; j < MAP_COLS; j++) {
			int tileX = j * TILE_SIZE;
			int tileY = i * TILE_SIZE;
			int tileColor = map[i][j] != 0 ? 255 : 0;
			SDL_SetRenderDrawColor(renderer, tileColor, tileColor, tileColor, 255);
			SDL_Rect mapTile = {
				tileX * MINIMAP_SCALE_FACTOR,
				tileY * MINIMAP_SCALE_FACTOR,
				TILE_SIZE * MINIMAP_SCALE_FACTOR,
				TILE_SIZE * MINIMAP_SCALE_FACTOR
			};

			SDL_RenderFillRect(renderer, &mapTile);
		}
	}
}

void renderRays(void) {
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	for (int i = 0; i < NUM_RAYS; i++) {
		SDL_RenderDrawLine(
			renderer,
			player.x * MINIMAP_SCALE_FACTOR,
			player.y * MINIMAP_SCALE_FACTOR,
			rays[i].wallHitX * MINIMAP_SCALE_FACTOR,
			rays[i].wallHitY * MINIMAP_SCALE_FACTOR
		);
	}
}

void processInput(void) {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
		case SDL_QUIT: {
			isGameRunning = FALSE;
			break;
		}

		case SDL_KEYDOWN: {
			if (event.key.keysym.sym == SDLK_ESCAPE)
				isGameRunning = FALSE;
			if (event.key.keysym.sym == SDLK_UP)
				player.walkDirection = 1;
			if (event.key.keysym.sym == SDLK_DOWN)
				player.walkDirection = -1;
			if (event.key.keysym.sym == SDLK_RIGHT)
				player.turnDirection = 1;
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

void update(void) {
	// sleep until target frame time length is reached
	int timeToWait = FRAME_TIME_LENGTH - (SDL_GetTicks() - ticksLastFrame);

	if (timeToWait > 0 && timeToWait <= FRAME_TIME_LENGTH) {
		SDL_Delay(timeToWait);
	}

	float deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;

	ticksLastFrame = SDL_GetTicks();
	
	movePlayer(deltaTime);
	castAllRays();
}

void clearColorBuffer(Uint32 color) {
	for (int x = 0; x < WINDOW_WIDTH; x++) {
		for (int y = 0; y < WINDOW_HEIGHT; y++) {
			if (x == y)
				colorBuffer[(WINDOW_WIDTH * y) + x] = color;
			else
				colorBuffer[(WINDOW_WIDTH * y) + x] = 0xFFFF0000;
		}
	}
}

void renderColorBuffer(void) {
	SDL_UpdateTexture(
		colorBufferTexture, 
		NULL,
		colorBuffer,
		(int)((Uint32)WINDOW_WIDTH * sizeof(Uint32))
	);

	SDL_RenderCopy(
		renderer,
		colorBufferTexture,
		NULL,
		NULL
	);


}

void render(void) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	renderColorBuffer();
	// Clear color buffer with black
	//clearColorBuffer(0xFF000000);
	clearColorBuffer(0xFF00EE30);

	// Display minimap
	renderMap();
	renderRays();
	renderPlayer();

	SDL_RenderPresent(renderer);
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