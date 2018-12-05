#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_BUF 128

main() {

	int n, fd[2];
	pid_t pid;
	char buf[MAX_BUF];

	if (pipe(fd) < 0) {
		perror("pipe");
		exit(1);
	}

	if ((pid = fork()) < 0) {
		perror("fork");
		exit(1);
	}

	else if (pid == 0) {
		printf("Child is using %d for writing.\n", fd[1]);
		printf("Child is using %d for reading.\n", fd[0]);
		close(fd[1]);
		if ((n = read(fd[0], buf, MAX_BUF)) < 0) {
			perror("read");
			exit(1);
		}

		printf("Child : Received data from parent: ");
		fflush(stdout);
		write(STDOUT_FILENO, buf, n); // console에 parent로 부터 받은 data를 출력.
	} else {
		printf("Parent is using %d for wrintg.\n", fd[1]);
		printf("Parent is using %d for reading.\n", fd[0]);
		close(fd[0]);
		strcpy(buf, "Hello, World!\n");
		printf("Parent: Send data to child\n");
		printf("strlen(buf) : %d", strlen(buf)+1);
		write(fd[1], buf, strlen(buf)); // strlen(buf) + '\n' 을 자식에게 send해준다.
	}

	exit(0);

}
