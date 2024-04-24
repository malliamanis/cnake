#include <stdio.h>
#include <stdlib.h>

#include "cnake.h"

#define WIDTH 720
#define HEIGHT 720
#define SCALE 40

int main(int argc, char **argv)
{
	uint32_t width, height, scale;

	if (argc < 4) {
		fprintf(
			stderr,
			"no width, height and scale provided: using default settings:\n"
			"Width = %d\n"
			"Height = %d\n"
			"Scale = %d\n",
			(width  = WIDTH),
			(height = HEIGHT),
			(scale  = SCALE)
		);
	}
	else {
		width  = atoi(argv[1]);
		height = atoi(argv[2]);
		scale  = atoi(argv[3]);
	}

	if (width % scale != 0 || height % scale != 0) {
		fprintf(stderr, "error: both width and height must be evenly divisble by scale\n");
		return 1;
	}

	cnake_run(width, height, scale);

	return 0;
}
