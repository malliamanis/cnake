#include <stdlib.h>

#include "snake.h"

#define INITIAL_PIECES_SIZE 16

struct snake snake_create(uint32_t plane_width, uint32_t plane_height, vec2 initial_pos)
{
	struct snake s = {
		.plane_width = plane_width,
		.plane_height = plane_height,
		.length = 1,
		.pieces_size = INITIAL_PIECES_SIZE,
		.pieces = calloc(INITIAL_PIECES_SIZE, sizeof(*s.pieces)),
	};

	s.pieces[0] = (struct snake_piece) { .pos = initial_pos };

	return s;
}

void snake_tick(enum direction last_key_pressed, struct snake *s)
{
	// s->pieces[0].dir = last_key_pressed;

	enum direction last_dir = last_key_pressed;
	struct snake_piece *piece;
	for (int i = 0; i < s->length; ++i) {
		piece = &s->pieces[i];

		enum direction temp = piece->dir;
		piece->dir = last_dir;
		last_dir = temp;

		switch (piece->dir) {
			case DIRECTION_LEFT:
				--s->pieces[i].pos.x;
				break;
			case DIRECTION_RIGHT:
				++s->pieces[i].pos.x;
				break;
			case DIRECTION_UP:
				--s->pieces[i].pos.y;
				break;
			case DIRECTION_DOWN:
				++s->pieces[i].pos.y;
				break;
			default:
				break;
		}
	}
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

void snake_add_piece(struct snake *s)
{

}

void snake_destroy(struct snake *s)
{
	free(s->pieces);
	*s = (struct snake) {0};
}
