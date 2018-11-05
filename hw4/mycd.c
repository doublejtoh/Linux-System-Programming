#include <stdio.h>
#include <unistd.h>

main (int argc, char * argv[]) {
	if (argc!=2) {
		fprintf(stderr, "Usage: %s dirname\n", argv[0]);
		exit(1);
	}

	if (chdir(argv[1]) < 0 ) { // argv[1]로 현재 directory를 바꾸어줌. 실패시 -1 리턴.
		perror("chdir");
		exit(1);
	}
}


// ./mycd 실행해도 안바뀌는 이유: master shell process가 mycd실행해서 fork해서 다른 프로세스가current directory 바꾸고, 종료되면 master shell에서는 current directory가 변경되지 않음. 별개의 것임.
