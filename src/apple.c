#include <SDL2/SDL_render.h>
#include <inttypes.h>
#include <stdlib.h>

#include "apple.h"

vec2 apple_get_random_pos(struct snake *player)
{
	vec2 pos;
	bool inside_player;
	do {
		pos.x = rand() % player->plane_width;
		pos.y = rand() % player->plane_height;

		vec2 piece_pos;
		for (uint32_t i = 0; i < player->length; ++i) {
			piece_pos = player->pieces[i].pos;

			if (pos.x == piece_pos.x || pos.y == piece_pos.y)
				inside_player = true;
			else
				inside_player = false;
		}
	} while (inside_player);

	return pos;
}

void apple_render(SDL_Renderer *renderer, vec2 apple_pos)
{
	SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderDrawPoint(renderer, apple_pos.x, apple_pos.y);
}
