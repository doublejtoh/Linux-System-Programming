#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

main (int argc, char * argv[]) {
	struct stat statbuf;
	char * newpath;
	
	if (argc != 3) {
		fprintf(stderr, "Usage: $s old new\n", argv[0]);
		exit(1);
	}
	
	if (stat(argv[2], &statbuf) < 0 ) {
		perror("stat");
		exit(1);
	}

	if (S_ISDIR(statbuf.st_mode)) {
		strcpy(newpath, argv[2]);
		strcat(newpath, "/");
		strcat(newpath, argv[1]);
	} else {
		strcpy(newpath, argv[2]);
	}

	if ( rename(argv[1], newpath) < 0 ) { // argv[1]에서 argv[2]로 rename한다.
		perror("rename");
		exit(1);
	}
}
