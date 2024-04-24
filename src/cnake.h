#ifndef CNAKE_H
#define CNAKE_H

#include <stdint.h>

#define TICKS_PER_SECOND 7

typedef union {
	struct {
		uint32_t x;
		uint32_t y;
	};

	uint64_t bits;
} vec2;

void cnake_run(uint32_t width, uint32_t height, uint32_t scale);

#endif
