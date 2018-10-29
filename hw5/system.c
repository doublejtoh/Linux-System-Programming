#include <stdio.h>
#include <stdlib.h>

main () {

	int status;
	if ((status = system("date")) < 0) { // system은 fork하고, "date" command를 실행할 프로세를 exec하는데, 프로세스를 fork하는데 실패하면 -1를 리턴한다.
		perror("system");
		exit(1);
	}

	printf("exit status = %d\n", status);
	
	if ((status = system("nosuchcommand")) < 0) { // 32512 리턴
		perror("system");
		exit(1);
	}

	printf("exit status = %d\n", status);

	if ((status = system("who; exit 44;")) < 0) { // 누가 로그인 되어있는지 출력하고, exit. 11264 리턴.
		perror("system");
		exit(1);
	}

	printf("exit status = %d\n", status);
}
