#include <stdio.h>
#include <unistd.h>

main(int argc, char * argv[]) {
	if (argc != 3 ) {
		fprintf(stderr, "USage: $s source destination \n", argv[0]);
		exit(1);
	}

	if ( symlink(argv[1], argv[2]) < 0 ) { // symbolic link. (바로가기)
		perror("link");
		exit(1);
	}

}
