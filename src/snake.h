#ifndef SNAKE_H
#define SNAKE_H

#include <stddef.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

#include "cnake.h"

typedef enum {
	DIRECTION_NULL,
	DIRECTION_LEFT,
	DIRECTION_RIGHT,
	DIRECTION_UP,
	DIRECTION_DOWN
} Direction;

typedef struct {
	vec2 pos;
	Direction dir;
} SnakePiece;

typedef struct {
	uint32_t plane_width, plane_height;

	uint32_t score, best_score;

	uint32_t length;
	size_t pieces_size;
	SnakePiece *pieces;

	vec2 head_initial_pos;
	vec2 head_prev_pos;

	bool dead;
	uint32_t death_timer;
} Snake;

Snake snake_create(uint32_t plane_width, uint32_t plane_height, vec2 initial_pos);

void snake_tick(Direction last_key_pressed, Snake *s);
void snake_add_piece(Snake *s);

void snake_render(SDL_Renderer *renderer, Snake *s);

void snake_destroy(Snake *s);

#endif

