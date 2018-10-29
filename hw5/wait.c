#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

main () {

	pid_t pid;
	int status;

	if ((pid = fork()) < 0 ) {
		perror("fork");
		exit(1);
	}

	else if (pid == 0) {
		printf("I m  a child\n");
		sleep(2);
	}
	else {
		wait(&status); //waits for the first child to die.  The  return  value is that of the wait(2) system call.
		printf("A  child killed with %d status\n", status);
		if(WIFEXITED(status)) {
			printf("normally exited");
		}
	}
}
