#include <stdio.h>
#include <stdlib.h>
#define MAX_CMD 256

main() {

	char cmd[MAX_CMD]; // cmd입력을 받을 변수
	int pid; // process id
	int status; // system() 의 return value를 위한 변수
	while(1) {
		printf("CMD> ");
		fgets(cmd, MAX_CMD, stdin); // 사용자로부터 입력을 받아 cmd에 저장.
		if ((pid = fork()) < 0) { // process fork.
			perror("fork");
			exit(1);
		}

		else if (pid == 0) { // 자식 process에서는 
			if ((status = system(cmd)) < 0) { //system(cmd)를 통해서 cmd를 수행함.
				perror("system"); // 만약 system의 return value가 < 0 이라면 system에러를 출력.
				exit(1);
			}
			
		} else { // 부모 process에서는 waitpid(pid)를 통해서 자식 프로세스가 끝나기를 기다림.
			waitpid(pid, NULL, 0);
		}
	}
}
