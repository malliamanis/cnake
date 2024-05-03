#include <SDL2/SDL_timer.h>
#include <time.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "cnake.h"
#include "snake.h"
#include "apple.h"

#define TITLE "Cnake"

void cnake_run(uint32_t window_width, uint32_t window_height, uint32_t render_scale)
{
	/* INIT */

	srand(time(NULL));
	SDL_Init(SDL_INIT_VIDEO);

	uint32_t width = window_width / render_scale;
	uint32_t height = window_height / render_scale;
	bool quit = false;

	Direction last_key_pressed;
	Snake player = snake_create(width, height, (vec2) { .x = width >> 1, .y = height >> 1 });
	vec2 apple_pos = apple_get_random_pos(&player);

	SDL_Window *window = SDL_CreateWindow(
		TITLE,
		SDL_WINDOWPOS_CENTERED_DISPLAY(0),
		SDL_WINDOWPOS_CENTERED_DISPLAY(0),
		window_width,
		window_height,
		SDL_WINDOW_ALLOW_HIGHDPI
	);

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
	SDL_RenderSetScale(renderer, render_scale, render_scale);

	const double time_step_scaled = (1.0 / TICKS_PER_SECOND) * SDL_GetPerformanceFrequency();

	double currentTime = SDL_GetPerformanceCounter();
	double newTime;
	double accumulator = 0;

	while (!quit) {
		/* UPDATE */

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				quit = true;
			else if (event.type == SDL_KEYDOWN) {
				// it's easier to put the player keyboard handling here instead in the snake.c file
				Direction head_dir = player.pieces[0].dir;
				switch (event.key.keysym.scancode) {
					case SDL_SCANCODE_ESCAPE:
						quit = true;
						break;
					case SDL_SCANCODE_A:
					case SDL_SCANCODE_LEFT:
						if (head_dir != DIRECTION_RIGHT)
							last_key_pressed = DIRECTION_LEFT;
						break;
					case SDL_SCANCODE_D:
					case SDL_SCANCODE_RIGHT:
						if (head_dir != DIRECTION_LEFT)
							last_key_pressed = DIRECTION_RIGHT;
						break;
					case SDL_SCANCODE_W:
					case SDL_SCANCODE_UP:
						if (head_dir != DIRECTION_DOWN)
							last_key_pressed = DIRECTION_UP;
						break;
					case SDL_SCANCODE_S:
					case SDL_SCANCODE_DOWN:
						if (head_dir != DIRECTION_UP)
						last_key_pressed = DIRECTION_DOWN;
						break;
					default:
						break;
				}
			}
		}

		newTime = SDL_GetPerformanceCounter();

		accumulator += newTime - currentTime;
		currentTime = newTime;

		while (accumulator >= time_step_scaled) {
			accumulator -= time_step_scaled;

			/* TICK */

			if (player.dead)
				last_key_pressed = DIRECTION_NULL;
			snake_tick(last_key_pressed, &player);

			if (player.pieces[0].pos.bits == apple_pos.bits) {
				snake_add_piece(&player);
				apple_pos = apple_get_random_pos(&player);
			}
		}

		/* RENDER */

		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(renderer);

		apple_render(renderer, apple_pos);
		snake_render(renderer, &player);

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();

	snake_destroy(&player);
}
