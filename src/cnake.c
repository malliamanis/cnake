#include <time.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "cnake.h"
#include "snake.h"
#include "apple.h"

#define TITLE "Cnake"

struct state {
	uint32_t width, height;

	bool quit;

	SDL_Window *window;
	SDL_Renderer *renderer;

	const uint8_t *keys;

	enum direction last_key_pressed;
	struct snake player;
	vec2 apple_pos;
};

static struct state s;

static void init(uint32_t width, uint32_t height, uint32_t scale);

static void update(void);
static void tick(void);

static void render(void);

static void destroy(void);

static void init(uint32_t window_width, uint32_t window_height, uint32_t render_scale)
{
	srand(time(NULL));
	SDL_Init(SDL_INIT_VIDEO);

	s = (struct state) {
		.width = window_height / render_scale,
		.height = window_height / render_scale,
		.quit = false,
	};

	s.window = SDL_CreateWindow(
		TITLE,
		SDL_WINDOWPOS_CENTERED_DISPLAY(0),
		SDL_WINDOWPOS_CENTERED_DISPLAY(0),
		window_width,
		window_height,
		SDL_WINDOW_ALLOW_HIGHDPI
	);

	s.renderer = SDL_CreateRenderer(s.window, -1, SDL_RENDERER_PRESENTVSYNC);
	SDL_RenderSetScale(s.renderer, render_scale, render_scale);

	s.player = snake_create(s.width, s.height, (vec2) { s.width >> 1, s.height >> 1 });
	s.apple_pos = apple_get_random_pos(&s.player);
}

void cnake_run(uint32_t window_width, uint32_t window_height, uint32_t render_scale)
{
	init(window_width, window_height, render_scale);

	uint64_t ticks_per_sec = 8;
	uint64_t delta_time = 1000 / ticks_per_sec;

	uint64_t currentTime = SDL_GetTicks64();
	uint64_t newTime;
	uint64_t accumulator = 0;

	while (!s.quit) {
		update();

		newTime = SDL_GetTicks64();

		accumulator += newTime - currentTime;
		currentTime = newTime;

		while (accumulator >= delta_time) {
			accumulator -= delta_time;

			tick();
		}

		render();
	}

	destroy();
}

static void update(void)
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT)
			s.quit = true;
		else if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.scancode) {
				case SDL_SCANCODE_ESCAPE:
					s.quit = true;
					break;
				case SDL_SCANCODE_A:
				case SDL_SCANCODE_LEFT:
					s.last_key_pressed = DIRECTION_LEFT;
					break;
				case SDL_SCANCODE_D:
				case SDL_SCANCODE_RIGHT:
					s.last_key_pressed = DIRECTION_RIGHT;
					break;
				case SDL_SCANCODE_W:
				case SDL_SCANCODE_UP:
					s.last_key_pressed = DIRECTION_UP;
					break;
				case SDL_SCANCODE_S:
				case SDL_SCANCODE_DOWN:
					s.last_key_pressed = DIRECTION_DOWN;
					break;
				default:
					break;
			}
		}
	}
}

static void tick(void)
{
	snake_tick(s.last_key_pressed, &s.player);
}

static void render(void)
{
	// clear background
	SDL_SetRenderDrawColor(s.renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(s.renderer);

	apple_render(s.renderer, s.apple_pos);
	snake_render(s.renderer, &s.player);

	SDL_RenderPresent(s.renderer);
}

static void destroy(void)
{
	SDL_DestroyRenderer(s.renderer);
	SDL_DestroyWindow(s.window);

	SDL_Quit();

	snake_destroy(&s.player);
	s = (struct state) {0};
}
