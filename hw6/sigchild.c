#include <signal.h>
#include <stdio.h>

void SigChldHandler(int signo) {
	printf("Received a SIGCHLD signal\n");
	exit(0);
}

main() {
	pid_t pid;

	if (signal(SIGCHLD, SigChldHandler) == SIG_ERR) {
		perror("signal");
	}

	if ((pid = fork()) < 0 ) {
		perror("fork");
		exit(1);
	}

	else if (pid == 0) {
		printf("child process with pid %d\n", getpid());
		exit(0);
	}
	else {
		while(1) {
			printf(" Parent says: i m working now....\n");
		}
		printf("parent process will be terminated.\n");
	}

}
