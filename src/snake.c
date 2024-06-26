#include <stdlib.h>

#include "snake.h"

static void death_tick(Snake *s);

#define PIECES_INITIAL_SIZE 16

Snake snake_create(uint32_t plane_width, uint32_t plane_height, vec2 initial_pos)
{
	Snake s = {
		.plane_width = plane_width,
		.plane_height = plane_height,
		.length = 0,
		.pieces_size = PIECES_INITIAL_SIZE,
		.pieces = calloc(PIECES_INITIAL_SIZE, sizeof(*s.pieces)),
		.head_initial_pos = initial_pos,
		.head_prev_pos = initial_pos,
	};

	for (uint32_t i = 0; i < 3; ++i) {
		s.pieces[i] = (SnakePiece) {
			.pos = initial_pos,
			.dir = DIRECTION_DOWN
		};
		s.pieces[i].pos.y -= i;

		++s.length;
	}

	return s;
}

void snake_tick(Direction last_key_pressed, Snake *s)
{
	if (s->dead) {
		death_tick(s);
		return;
	}

	s->head_prev_pos = s->pieces[0].pos;

	vec2 head_old_pos;
	Direction last_dir = last_key_pressed;

	for (uint32_t i = 0; i < s->length; ++i) {
		SnakePiece *piece = &s->pieces[i];

		Direction temp_dir = piece->dir;
		piece->dir = last_dir;
		last_dir = temp_dir;
		
		if (i == 0)
			head_old_pos = piece->pos;

		switch (piece->dir) {
			case DIRECTION_LEFT:
				if (piece->pos.x == 0)
					piece->pos.x = s->plane_width - 1;
				else
					--piece->pos.x;
				break;
			case DIRECTION_RIGHT:
				if (piece->pos.x == s->plane_width - 1)
					piece->pos.x = 0;
				else
					++piece->pos.x;
				break;
			case DIRECTION_UP:
				if (piece->pos.y == 0)
					piece->pos.y = s->plane_height - 1;
				else
					--piece->pos.y;
				break;
			case DIRECTION_DOWN:
				if (piece->pos.y == s->plane_height - 1)
					piece->pos.y = 0;
				else
					++piece->pos.y;
				break;
			case DIRECTION_NULL:
				return;
		}

		// dirty solution
		if (i == 0) {
			for (uint32_t j = 1; j < s->length; ++j) {
				if (s->pieces[0].pos.bits == s->pieces[j].pos.bits) {
					s->dead = true;
					s->pieces[0].pos = head_old_pos;
					return;
				}
			}
		}
	}
}

static void death_tick(Snake *s)
{
	if (++s->death_timer < TICKS_PER_SECOND)
		return;

	if (s->length > 1)
		s->pieces[--s->length] = (SnakePiece) {0};
	else {
		s->dead = false;
		s->death_timer = 0;
		s->score = 0;
	}
}

void snake_add_piece(Snake *s)
{
	if (s->length == s->pieces_size)
		s->pieces = realloc(s->pieces, (s->pieces_size *= 2) * sizeof(*s->pieces));

	vec2 new_pos = s->pieces[s->length - 1].pos;
	switch (s->pieces[s->length - 1].dir) {
		case DIRECTION_LEFT:
			++new_pos.x;
			break;
		case DIRECTION_RIGHT:
			--new_pos.x;
			break;
		case DIRECTION_UP:
			++new_pos.y;
			break;
		case DIRECTION_DOWN:
			--new_pos.y;
			break;
		default:
			break;
	}

	s->pieces[s->length++] = (SnakePiece) { .pos = new_pos };

	if (s->best_score < ++s->score)
		s->best_score = s->score;
	printf("\nBest score: %d\nCurrent score: %d\n", s->best_score, s->score);
	
}

void snake_render(SDL_Renderer *renderer, Snake *s)
{
	SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);

	// vec2 piece_pos;
	for (uint32_t i = 0; i < s->length; ++i) {
		// piece_pos = s->pieces[i].pos;
		SDL_RenderDrawPoint(renderer, s->pieces[i].pos.x, s->pieces[i].pos.y);
	}
}

void snake_destroy(Snake *s)
{
	free(s->pieces);
	*s = (Snake) {0};
}
