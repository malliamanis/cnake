#ifndef CNAKE_H
#define CNAKE_H

#include <stdint.h>

typedef struct {
	uint32_t x;
	uint32_t y;
} vec2;

void cnake_run(uint32_t width, uint32_t height, uint32_t scale);

#endif
