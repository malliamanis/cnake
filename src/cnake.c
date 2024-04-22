#include <stdlib.h>

#include <SDL2/SDL.h>

#include "cnake.h"

#define TITLE "Cnake"

struct state {
	uint32_t width, height, scale;

	bool quit;

	uint32_t *pixels;

	SDL_Window *window;
	SDL_Texture *screen;
	SDL_Renderer *renderer;

	const uint8_t *keys;
};

struct state s;

static void init(uint32_t width, uint32_t height, uint32_t scale);

static void update(void);
static void tick(void);

static void render(void);

static void destroy(void);

static void init(uint32_t width, uint32_t height, uint32_t scale)
{
	SDL_Init(SDL_INIT_VIDEO);

	s = (struct state) {
		.width = width / scale,
		.height = height / scale,
		.scale = scale,
		.quit = false,
	};

	s.pixels = calloc(s.width * s.height, sizeof(*s.pixels));

	s.window = SDL_CreateWindow(
		TITLE,
		SDL_WINDOWPOS_CENTERED_DISPLAY(0),
		SDL_WINDOWPOS_CENTERED_DISPLAY(0),
		width,
		height,
		SDL_WINDOW_ALLOW_HIGHDPI
	);

	s.renderer = SDL_CreateRenderer(s.window, -1, SDL_RENDERER_PRESENTVSYNC);

	s.screen = SDL_CreateTexture(
		s.renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		s.width,
		s.height
	);
}

void cnake_run(uint32_t width, uint32_t height, uint32_t scale)
{
	init(width, height, scale);

	uint64_t ticks_per_sec = 60;
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
	}

	s.keys = SDL_GetKeyboardState(NULL);
	if (s.keys[SDL_SCANCODE_ESCAPE])
		s.quit = true;
}

static void tick(void)
{
}

static void render(void)
{
	// clear background
	memset(s.pixels, 0, s.width * s.height * sizeof(*s.pixels));

	SDL_UpdateTexture(s.screen, NULL, s.pixels, s.width * sizeof(*s.pixels));
	SDL_RenderCopyEx(s.renderer, s.screen, NULL, NULL, 0.0, NULL, 0);

	SDL_RenderPresent(s.renderer);
}

static void destroy(void)
{
	SDL_DestroyTexture(s.screen);
	SDL_DestroyRenderer(s.renderer);
	SDL_DestroyWindow(s.window);

	SDL_Quit();

	free(s.pixels);
	s = (struct state) {0};
}
