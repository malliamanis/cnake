#ifndef SNAKE_H
#define SNAKE_H

#include <stddef.h>

#include <SDL2/SDL.h>

#include "cnake.h"

enum direction {
	DIRECTION_NULL,
	DIRECTION_LEFT,
	DIRECTION_RIGHT,
	DIRECTION_UP,
	DIRECTION_DOWN
};

struct snake_piece {
	vec2 pos;
	enum direction dir;
};

struct snake {
	uint32_t plane_width, plane_height;

	uint32_t score, best_score;

	uint32_t length;
	struct snake_piece *pieces;
	size_t pieces_size;

	vec2 head_initial_pos;
	vec2 head_prev_pos;

	bool dead;
	uint32_t death_timer;
};

struct snake snake_create(uint32_t plane_width, uint32_t plane_height, vec2 initial_pos);

void snake_tick(enum direction last_key_pressed, struct snake *s);
void snake_add_piece(struct snake *s);

void snake_render(SDL_Renderer *renderer, struct snake *s);

void snake_destroy(struct snake *s);

#endif
