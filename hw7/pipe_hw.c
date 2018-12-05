#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_BUF 128

main() {
	int n,child_telling_fd[2], parent_telling_fd[2];
	pid_t pid;
	char buf[MAX_BUF];

	if (pipe(child_telling_fd) < 0 || pipe(parent_telling_fd) < 0){ // child가 말하는 pipe한개와 parent가 말하는 pipe 총 2개의 pipe를 생성.
		perror("pipe");
		exit(1);
	}

	if ((pid = fork()) < 0) {
		perror("fork");
		exit(1);
	}

	else if (pid == 0) { // 자식인 경우
		close(parent_telling_fd[1]); // parent_telling_fd pipe의 write port를 닫음.
		close(child_telling_fd[0]); // child_telling_fd pipe의 read port를 닫음.
		

		// Child Wait for parent tell.
		printf("Child : Wait for parent to send data\n");
		if (( n = read(parent_telling_fd[0], buf, MAX_BUF)) < 0) {
			perror("read");
			exit(1);
		}

		printf("Child : Received data from parent: ");
		fflush(stdout);
		write(STDOUT_FILENO, buf, n);


		// Child write to parent.
		strcpy(buf, "I(child) is sending message to Parent!!!\n");
		printf("Child : Send data to parent\n");
		write(child_telling_fd[1], buf, strlen(buf)+1);

	} else { // 부모 인 경우
		close(parent_telling_fd[0]); // parent_telling_fd pipe의 read port를 닫음.
		close(child_telling_fd[1]); // child_telling_fd pipe의 write port를 닫음.

		
		// Parent write to child.
		strcpy(buf, "I(parent) is sending message to Child!!!\n");
		printf("Parent : send data to child\n");
		write(parent_telling_fd[1], buf, strlen(buf)+1);

		// Parent wait for child tell.
		if (( n = read(child_telling_fd[0], buf, MAX_BUF)) < 0) {
			perror("read");
			exit(1);
		}

		printf("Parent: Received data from child: %s\n", buf);
	}

}
