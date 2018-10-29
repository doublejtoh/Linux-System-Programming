#include <stdio.h>
#include <unistd.h>

#define MAX_BUF 256

main(int argc, char * argv[]) {
	char buf[MAX_BUF];
	
	if (getcwd(buf, MAX_BUF) == NULL) { // current working directory의 absolute path를 가져온다. 에러가 나면 null를 리턴
		perror("getcwd");
		exit(1);
	}

	printf("%s\n", buf);
}
