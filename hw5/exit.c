#include <stdio.h>
#include <stdlib.h>

void myexit1() {
	printf("First exit handler\n");
}

void myexit2() {
	printf("second exit handler\n");
}

main() {
	if (atexit(myexit2) != 0) { // myexit2함수를 process 종료시 실행하겠다.
		perror("atexit");
		exit(1);
	}

	if (atexit(myexit1) != 0) {
		perror("atexit");
		exit(1);
	}

	if (atexit(myexit1) != 0) {
		perror("atexit");
		exit(1);
	}
	printf("Main is done\n"); // 이후에 myexit1이 호출되고, myexit2가 호출 됨.
/*

Main is done
First exit handler
First exit handler
second exit handler

*/

}
