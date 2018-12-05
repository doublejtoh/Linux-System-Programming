#include <stdio.h>

main () {

	float x[4] = {1, 1, 1, 1};
	float y[4] = {2,2,2,2};

	float * total = malloc(8 * sizeof(float));

	memcpy(total, x, 4 * sizeof(float));

	memcpy(total + 4, y, 4 * sizeof(float));

	for (int i = 0 ; i < 8; i++) {

		printf("%f\n", total[i]);
	}
}
