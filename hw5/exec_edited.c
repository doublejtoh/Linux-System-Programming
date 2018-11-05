#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

main() {

	pid_t pid;
	char * argv[] = { "mycp", "exec.c", "exec2.c", NULL };

	// exec1
	if ((pid = fork()) < 0) {
		perror("fork");
		exit(1);
	}

	else if (pid == 0) {
		if (execl("/home/lsp41/hw3/mycp", "mycp", "exec.c", "exec1.c", NULL) < 0) { // mycp를 실행하는데, list로 argument를 주겠다.
			perror("execl");
			exit(1);
		}

	}

	if (waitpid(pid, NULL, 0) < 0) { // 자식 프로세스를 기다린다.
		perror("waitpid");
		exit(1);
	}

	//execv
	if ((pid = fork()) < 0) {
		perror("fork");
		exit(1);
	}

	else if (pid == 0) {
		if (execv("/home/lsp41/hw3/mycp", argv) < 0) { // mycp를 실행하는데, vector로 argument를 주겠다.
			perror("execv");
			exit(1);
		}
	}

	if (waitpid(pid, NULL, 0) < 0) { // 자식 프로세스의 종료를 기다린다.
		perror("waitpid");
		exit(1);
	}

	//execlp
	if ((pid = fork()) < 0) {
		perror("fork");
		exit(1);
	}

	else if (pid == 0) {
		if (execlp("env", "env", NULL) < 0) { // env path로부터 env파일을 실행하는데, argument는 list로 주겠다.
			perror("execlp");
			exit(1);
		}
	}

	if (waitpid(pid, NULL, 0) < 0) { // 자식 프로세스의 종료를 기다린다.
		perror("waitpid");
		exit(1);
	}
		
	
 
}
