#include <stdio.h>
#include <unistd.h>

main (int argc, char * argv[]) {
	if ( argc != 3 ) {
		fprintf(stderr, "Usage: %s source destination\n " , argv[0]);
		exit(1);
	}

	if ( link(argv[1], argv[2]) < 0 ) { // argv[1] 파일을 argv[2] 에 link 해줌
		perror("link");
		exit(1);
	}
}
