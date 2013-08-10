#include "random_time.h"
#include <sys/time.h>
#include <cstdlib>

void seed_init() {
	struct timeval t;
	gettimeofday(&t, NULL);
	srand(t.tv_sec + t.tv_usec);
}

int random_int() {
	static int left = 0;
	if (left <= 0) {
		seed_init();
		left = rand() % 20 + 2;
	}
	left -= 1;
	return rand();
}

float norm_rand() {
	return (float)random_int() / (float)RAND_MAX;
}

float cont_intrv_rand (float a, float b) {
	return norm_rand() * (b - a) + a;
}

float random_time (float medium_time) {
	return cont_intrv_rand(medium_time * 0.5, medium_time * 1.5);
}
