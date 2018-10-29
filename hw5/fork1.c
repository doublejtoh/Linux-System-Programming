#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

main() {

	int pid;
	if ((pid = fork()) < 0) // creates a new process. return value is 0 in the child process, parent return value is the process id of child process. -1 return : error 
	 {
		perror("fork");
		exit(1);
	}

	else if (pid == 0) { // child process
		printf("I am %d. My parent is %d.\n", getpid(), getppid());
	
	} else { // parent process.
		printf("I am %d, My child is %d.\n", getpid(), pid);
	}
}
