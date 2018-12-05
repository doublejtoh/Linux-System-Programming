#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

static int Pfd1[2], Pfd2[2];
//Pfd1는 부모가 write하고, 자식이 read하는 pipe.
//Pfd2는 자식이 write하고, 부모가 read하는 pipe.
void TELL_WAIT(void) {
	if (pipe(Pfd1) < 0 || pipe(Pfd2) < 0) {
		perror("pipe");
		exit(1);
	}
}

void TELL_PARENT(void) {
	if (write(Pfd2[1], "c", 1) != 1) { //Pfd2[1]으로 "c"를 write함.
		perror("write");
		exit(1);
	}
}

void WAIT_PARENT(void) {
	char c;
	if (read(Pfd1[0], &c, 1) != 1) {
		perror("read");
		exit(1);
	}

	if (c != 'p') {
		fprintf(stderr, "WAIT_PARENT: incorrect data");
		exit(1);
	}

}

void TELL_CHILD(void) {
	if (write(Pfd1[1], "p", 1) != 1) {
		perror("write");
		exit(1);
	}
}

void WAIT_CHILD(void) {
	char c;
	if (read(Pfd2[0], &c, 1) != 1) {
		perror("read");
		exit(1);
	}

	if (c != 'c') {
		fprintf(stderr, "WAIT_CHILD: incorrect data");
		exit(1);
	}
}
