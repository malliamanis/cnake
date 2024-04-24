#include <stdlib.h>

#include "snake.h"

static void death_tick(struct snake *s);
static void reset_pos(struct snake *s);

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

	reset_pos(&s);

	return s;
}

void snake_tick(enum direction last_key_pressed, struct snake *s)
{
	if (s->dead) {
		death_tick(s);
		return;
	}

	s->head_prev_pos = s->pieces[0].pos;

	vec2 head_new_pos;
	enum direction last_dir = last_key_pressed;

	for (uint32_t i = 0; i < s->length; ++i) {
		struct snake_piece *piece = &s->pieces[i];

		if (piece->pos.bits == head_new_pos.bits && i != 0)
			s->dead = true;

		enum direction temp_dir = piece->dir;
		piece->dir = last_dir;
		last_dir = temp_dir;

		switch (piece->dir) {
			case DIRECTION_LEFT:
				--piece->pos.x;
				break;
			case DIRECTION_RIGHT:
				++piece->pos.x;
				break;
			case DIRECTION_UP:
				--piece->pos.y;
				break;
			case DIRECTION_DOWN:
				++piece->pos.y;
				break;
			default:
				break;
		}
		
		if (i == 0)
			head_new_pos = piece->pos;
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
		reset_pos(s);
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
}

static void reset_pos(struct snake *s)
{
	s->pieces[0] = (struct snake_piece) { .pos = s->head_initial_pos };
}

void snake_render(SDL_Renderer *renderer, struct snake *s)
{
	SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);

	vec2 piece_pos;
	for (uint32_t i = 0; i < s->length; ++i) {
		piece_pos = s->pieces[i].pos;
		SDL_RenderDrawPoint(renderer, piece_pos.x, piece_pos.y);
	}
}

void snake_destroy(struct snake *s)
{
	free(s->pieces);
	*s = (struct snake) {0};
}
