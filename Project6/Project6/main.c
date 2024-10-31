#include <stdio.h>
#include <SDL.h>
#include "Constants.h"

// Variables globales
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int game_is_running = 1;

int last_frame_time = 0;
void draw_triangle(SDL_Renderer* renderer, int base_x, int base_y, int height);

int color_r = 50, color_g = 150, color_b = 75;

struct Ellipse
{
	float x, y;
	float radius_x, radius_y;
	int vel_x, vel_y;
} ellipse1, ellipse2;

struct Triangle
{
	float base_x, base_y;
	int height;
	int vel_x, vel_y;
} triangle;

int initialize_window(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error initializing SDL.\n");
		return 0;
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
		return 0;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		fprintf(stderr, "Error creating SDL renderer.\n");
		return 0;
	}

	return 1;
}

void process_input() {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
	case SDL_QUIT:
		game_is_running = 0;
		break;
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE) {
			game_is_running = 0;
		}
		break;
	}
}

void setup() {
	ellipse1 = (struct Ellipse){ 50, 50, 30, 20, VELOCIDADX, VELOCIDADY };
	ellipse2 = (struct Ellipse){ 300, 100, 20, 15, -VELOCIDADX, VELOCIDADY };
	triangle = (struct Triangle){ 200, 200, 30, -VELOCIDADX, -VELOCIDADY };
}

void update() {
	int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);

	if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
		SDL_Delay(time_to_wait);
	}

	float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;
	last_frame_time = SDL_GetTicks();

	ellipse1.x += ellipse1.vel_x * delta_time;
	ellipse1.y += ellipse1.vel_y * delta_time;
	ellipse2.x += ellipse2.vel_x * delta_time;
	ellipse2.y += ellipse2.vel_y * delta_time;
	triangle.base_x += triangle.vel_x * delta_time;
	triangle.base_y += triangle.vel_y * delta_time;

	// Colisiones con los bordes para elipses
	if (ellipse1.x >= WINDOW_WIDTH || ellipse1.x < 0) ellipse1.vel_x = -ellipse1.vel_x;
	if (ellipse1.y >= WINDOW_HEIGHT || ellipse1.y < 0) ellipse1.vel_y = -ellipse1.vel_y;
	if (ellipse2.x >= WINDOW_WIDTH || ellipse2.x < 0) ellipse2.vel_x = -ellipse2.vel_x;
	if (ellipse2.y >= WINDOW_HEIGHT || ellipse2.y < 0) ellipse2.vel_y = -ellipse2.vel_y;

	// Colisiones con los bordes para triángulo
	if (triangle.base_x >= WINDOW_WIDTH || triangle.base_x < 0) triangle.vel_x = -triangle.vel_x;
	if (triangle.base_y >= WINDOW_HEIGHT || triangle.base_y < 0) triangle.vel_y = -triangle.vel_y;

	// Cambiar colores progresivamente
	color_r = (color_r + 1) % 256;
	color_g = (color_g + 2) % 256;
	color_b = (color_b + 3) % 256;
}

void render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	// Dibujar elipses
	SDL_SetRenderDrawColor(renderer, color_r, color_g, color_b, 255);
	for (int w = 0; w < ellipse1.radius_x * 2; w++) {
		for (int h = 0; h < ellipse1.radius_y * 2; h++) {
			int dx = ellipse1.radius_x - w;
			int dy = ellipse1.radius_y - h;
			if ((dx * dx * 4) + (dy * dy * 4) <= (ellipse1.radius_x * ellipse1.radius_y)) {
				SDL_RenderDrawPoint(renderer, ellipse1.x + dx, ellipse1.y + dy);
			}
		}
	}
	for (int w = 0; w < ellipse2.radius_x * 2; w++) {
		for (int h = 0; h < ellipse2.radius_y * 2; h++) {
			int dx = ellipse2.radius_x - w;
			int dy = ellipse2.radius_y - h;
			if ((dx * dx * 4) + (dy * dy * 4) <= (ellipse2.radius_x * ellipse2.radius_y)) {
				SDL_RenderDrawPoint(renderer, ellipse2.x + dx, ellipse2.y + dy);
			}
		}
	}

	// Dibujar triángulo
	SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
	draw_triangle(renderer, triangle.base_x, triangle.base_y, triangle.height);

	SDL_RenderPresent(renderer);
}

void draw_triangle(SDL_Renderer* renderer, int base_x, int base_y, int height) {
	int half_base = height / 2;
	SDL_RenderDrawLine(renderer, base_x, base_y, base_x - half_base, base_y + height);
	SDL_RenderDrawLine(renderer, base_x, base_y, base_x + half_base, base_y + height);
	SDL_RenderDrawLine(renderer, base_x - half_base, base_y + height, base_x + half_base, base_y + height);
}

void destroy_window() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int main(int argc, char* args[]) {
	game_is_running = initialize_window();
	setup();

	while (game_is_running) {
		process_input();
		update();
		render();
	}

	destroy_window();
	return 0;
}
