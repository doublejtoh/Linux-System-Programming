#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int Var = 6;
char Buf[] = "a write to stdout \n";

main(){

	int var;
	pid_t  pid;
	var = 88;
	printf("%d\n",sizeof(Buf));
	write(STDOUT_FILENO, Buf, sizeof(Buf)-1); // STDOUT_FILENO: 1. sizeof(Buf)-2하면 "\n"이 짤리게된다.
	printf("Before fork\n");

	if ((pid = fork()) < 0) {
		perror("fork");
		exit(1);
	} else if (pid == 0) { // 자식 프로세스이면
		Var++;
		var++;
	} else {
		sleep(2); // 2초동안 pause
	}
	printf("pid = %d, Var = %d, var = %d\n", getpid(), Var, var);
}
