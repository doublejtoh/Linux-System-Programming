#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

main (int argc, char * argv[]) {
	if (argc != 2) {
		fprintf(stderr, " Usage: $s dirname \n", argv[0]);
		exit(1);
	}

	if (mkdir(argv[1], 0755) < 0) { // Owner R,W,X 가능하고, Group과 Others는 Read와 execute만 가능하게 dir를 하나 만들어라.
		perror("mkdir");
		exit(1);
	}
}
