#include <stdlib.h>

#include "snake.h"

static void death_tick(struct snake *s);

struct snake snake_create(uint32_t plane_width, uint32_t plane_height, vec2 initial_pos)
{
	const size_t initial_pieces_size = 16;

	struct snake s = {
		.plane_width = plane_width,
		.plane_height = plane_height,
		.length = 1,
		.pieces = calloc(initial_pieces_size, sizeof(*s.pieces)),
		.pieces_size = initial_pieces_size,
		.head_initial_pos = initial_pos,
		.head_prev_pos = initial_pos
	};

	s.pieces[0] = (struct snake_piece) { .pos = initial_pos };

	return s;
}

void snake_tick(enum direction last_key_pressed, struct snake *s)
{
	if (s->dead) {
		death_tick(s);
		return;
	}

	s->head_prev_pos = s->pieces[0].pos;

	vec2 head_old_pos;
	enum direction last_dir = last_key_pressed;

	for (uint32_t i = 0; i < s->length; ++i) {
		struct snake_piece *piece = &s->pieces[i];

		enum direction temp_dir = piece->dir;
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
			for (int j = 1; j < s->length; ++j) {
				if (s->pieces[0].pos.bits == s->pieces[j].pos.bits) {
					s->dead = true;
					s->pieces[0].pos = head_old_pos;
					return;
				}
			}
		}
	}
}

static void death_tick(struct snake *s)
{
	if (++s->death_timer < TICKS_PER_SECOND)
		return;

	if (s->length > 1)
		s->pieces[--s->length] = (struct snake_piece) {0};
	else {
		s->dead = false;
		s->death_timer = 0;
		s->score = 0;
	}
}

void snake_add_piece(struct snake *s)
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

	s->pieces[s->length++] = (struct snake_piece) { .pos = new_pos };

	if (s->best_score < ++s->score)
		s->best_score = s->score;
	printf("\nBest score: %d\nCurrent score: %d\n", s->best_score, s->score);
	
}

void snake_render(SDL_Renderer *renderer, struct snake *s)
{
	SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);

	// vec2 piece_pos;
	for (uint32_t i = 0; i < s->length; ++i) {
		// piece_pos = s->pieces[i].pos;
		SDL_RenderDrawPoint(renderer, s->pieces[i].pos.x, s->pieces[i].pos.y);
	}
}

void snake_destroy(struct snake *s)
{
	free(s->pieces);
	*s = (struct snake) {0};
}
