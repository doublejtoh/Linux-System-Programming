#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

char * EnvInit [] = { "USER=unknown", "PATH=/tmp", NULL} ; // 끝에 NULL을 넣은 이유? {}가 배열을 나타내는 것인지?
// {}는 pointer 배열을 나타낸다.
// 끝에 NULL을 넣은 이유는 나중에 execle의 마지막 파라미터인 envp의 끝을 알려주기 위해서.

main () {

	pid_t pid;
	if ((pid = fork()) < 0) {
		perror("fork");
		exit(1);
	}

	else if (pid == 0) {
		if (execle("/home/lsp41/hw5/env", "env", "myarg1", "asdf", NULL, EnvInit) < 0) { // /home/lsp41/hw5/env를 실행하는데, "env", "myarg1", "asdf" 를 argument로 실행하고, EnvInit를 환경변수로 제공하겠다. 
			perror("execle");
			exit(1);
		}
		printf("%s\n", "자식 프로세스 아직 안끝남.");
	}

	printf("첫번째 fork 후 pid = %ld\n", (long) pid);

	if (waitpid(pid, NULL, 0) < 0) { // 자식 process가 끝날 때 까지 기다리는데, &status: NULL, options: 0으로 주겠다. 실패하면 이 if문으로 들어오라.
		perror("waitpid");
		exit(1);
	}

	if ((pid = fork()) < 0) {
		perror("fork");
		exit(1);
	}

	else if (pid == 0) {
		if (execlp("env", "env", NULL) < 0) { // file name을 줘야함. error 발생시 return value = -1
			perror("execlp");
			exit(1);
		}
		printf("%s\n", "자식 프로세스2 아직 안끝남");
	}

	else {
		printf("%s\n", "부모 프로세스 나 아직 안끝낫다.");
		exit(0);
	}

}
