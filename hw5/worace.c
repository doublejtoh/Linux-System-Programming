#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

void CharAtaTime(char * str) {
	char * ptr;
	int c,i;
	setbuf(stdout, NULL);
	for (ptr = str; c = *ptr++;) {
		for (i = 0; i< 999999; i++) {
			;
		}
		putc(c, stdout);
	}

}

main() {

	pid_t pid;
	if ((pid = fork()) < 0) {
		perror("fork");
		exit(1);
	} else if (pid == 0) {
		CharAtaTime("output from child\n");
	} else {
		wait(NULL); // 자식 프로세스가 끝날 떄 까지 기다린다.
		CharAtaTime("output from parent\n");
	}
}
