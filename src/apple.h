#ifndef APPLE_H
#define APPLE_H

#include "SDL2/SDL.h"

#include "snake.h"

vec2 apple_get_random_pos(Snake *player);
void apple_render(SDL_Renderer *renderer, vec2 apple_pos);

#endif
