#include <stdio.h>


int update(long * ptr) {

	return ((*ptr)++);
}


main() {
	int num = 10;
	int counter;
	int * a = &num;
	*a = 0;
	counter = update((long *) a);
	printf("a: %d\n", *a);
	printf("counter: %d\n", counter);
}
