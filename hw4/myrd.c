#include <stdio.h>
#include <unistd.h>

main (int argc, char * argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s dirname\n", argv[0]);
		exit(1);
	}

	if (rmdir(argv[1]) < 0) { // argv[1] directory를 지움.
		perror("rmdir");	
		exit(1);
	}
}
