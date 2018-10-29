#include <stdio.h>

main (int argc, char * argv[]) {
	if (argc != 3) {
		fprintf(stderr, "Usage: $s old new\n", argv[0]);
		exit(1);
	}

	if ( rename(argv[1], argv[2]) < 0 ) { // argv[1]에서 argv[2]로 rename한다.
		perror("rename");
		exit(1);
	}
}
