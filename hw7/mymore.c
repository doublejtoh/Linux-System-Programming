#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {

	int fd[2];
	pid_t child;
	FILE * fp;
	char  c_before[256], c_after[256], p_before[256], p_after[256];
	if (pipe(fd) == -1) { // pipe하나 생성.
		perror("pipe");
		exit(1);
	}

	child = fork();

	if ((fp = fopen("mymore_fd.txt", "wt")) == NULL) {
		perror("fopen");
		exit(1);
	}

	if (child == (pid_t)(-1)) {
		perror("fork");
		exit(1);
	}

	else if (child == (pid_t) 0) {
		close(1); // STDOUT descriptor를 닫는다.
		close(fd[0]); //자식이므로 read fd를 닫는다.
		sprintf(c_before, "c_before_fd[1]: %d\n", fd[1]);
		fputs(c_before, fp);
		if (dup(fd[1]) == -1) { // close된 lowest file descriptor를 copy해서 fd[1]에 넣는다.
			perror("dup");
			exit(1);
		}
		sprintf(c_after, "c_after_fd[1]: %d\n", fd[1]);
		fputs(c_after, fp);
		if ((execlp("ls", "ls", "-l", NULL)) == -1) { // ls -l 실행.
			perror("execlp");
			exit(1);
		}
	}
	else {
		close(0); // STDIN descriptor를 닫는다.
		close(fd[1]); // 부모이므로 write fd를 닫는다.
		sprintf(p_before, "p_before_fd[0]: %d\n", fd[0]);
		fputs(p_before, fp);
		if (dup2(fd[0], 0) == -1) { // close된 lowest file descriptor로 copy해서fd[0]에 넣는다.
			perror("dup");
			exit(1); 

		}

		sprintf(p_after, "p_after_fd[0]: %d\n", fd[0]);
		fputs(p_after, fp);
		if ((execlp("more", "more", NULL)) == -1) {
			perror("execlp");
			exit(1);
		}
	}
	fclose(fp);
	return 0;
}


/*

질문1 :
자식 프로세스 close(fd[0])하고나서, interrupt걸려서 부모의 close(fd[1])를 하고, dup(fd[1])하게 되면, fd[1]는 STDOUT descriptor를 copy하게 되지 않나? 
답: process별로 file descriptor(0,1,2, ....)가 있다. 독립적인것이므로 race condition 고려할 필요가 없다.


질문 2:
file write가 안되는데 이유가 궁금
*/
