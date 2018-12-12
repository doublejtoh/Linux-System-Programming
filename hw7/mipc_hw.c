#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include "synclib.h"


#define NLOOPS		10
#define SIZE		sizeof(long)
#define SHM_SIZE	100000
#define SHM_MODE	(SHM_R | SHM_W)

int update(long * ptr) {
	return ((*ptr)++);
}

main() {

	int i, counter;
	int shmid;
	int * pInt;
	char * shmptr;
	pid_t pid;

	if ((shmid = shmget(IPC_PRIVATE, SHM_SIZE, SHM_MODE)) < 0) {
		perror("shmget");
		exit(1);
	}

	if ((shmptr = shmat(shmid, 0, 0)) == (void *) -1 ) {
		perror("shmat");
		exit(1);
	}

	pInt = (int *) shmptr;
	TELL_WAIT(); // pipe 두개 init.
	if (( pid = fork()) < 0) {
		perror("fork");
		exit(1);
	}

	else if ( pid > 0) {
		for ( i = 0; i < NLOOPS; i += 2) {
			if ((counter = update((long *) pInt)) != i) {
				fprintf(stderr, "Counter mismatch\n");
				exit(1);
			}
			printf("Parent: counter=%d\n", counter);
			TELL_CHILD();
			WAIT_CHILD();
		}
	}

	else {
		for ( i = 1; i < NLOOPS; i += 2) {
			WAIT_PARENT();
			if ((counter = update((long *) pInt)) != i) {
				fprintf(stderr, "Counter mismatch\n");
				exit(1);
			}

			printf("Child : counter=%d\n", counter);
			TELL_PARENT();
		}
	}
}


