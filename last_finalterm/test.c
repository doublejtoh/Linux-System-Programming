#include <stdio.h>
#include <sys/time.h>


main() {
	struct timespec ts;
	struct timeval tv;

	gettimeofday(&tv, NULL);

	printf("%d\n",tv.tv_sec);
	sleep(2);
	gettimeofday(&tv, NULL);
	printf("%d\n", tv.tv_sec);
}
