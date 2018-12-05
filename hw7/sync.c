#include <stdio.h>
#include "synclib.h"

#define NLOOPS 5

main () {

	int i;
	pid_t pid;
	
	TELL_WAIT(); // pipe 2개를 만든다.
	if ((pid = fork()) < 0) {
		perror("fork");
		exit(1);
	}

	else if (pid > 0) {
		for (i = 0; i < NLOOPS; i++) {
			TELL_CHILD(); //Pfd1[1]에 "p"를 write.
			printf("Parent: Tell to child\n");
			printf("Parent: Wait for child to tell\n");
			WAIT_CHILD(); //Pfd2[0]에서 "c"를 read. 기다림. 즉, 이떄 wait상태로 변함.
		}
	} else {
		for (i = 0; i < NLOOPS; i++) {
			printf("Child: Wait for parent to tell\n");
			WAIT_PARENT(); // Pfd1[0]에서 "p"를 read. "p"를 기다림
			TELL_PARENT(); // Pfd2[1]에서 "c"를 write.
			printf("Child: Tell to parent\n");
		}
	}
	
}


/***** 실행 순서

line sequence
19		(1)		(11)	(18)	(25)	(33)
20		(2)		(12)	(19)	(26)	(34)
21		(3)		(15)	(20)	(29)	(37)
22		(4)		(16)	(21)	(32)	(38)

26		(5)		(9)		(17)	(27)	(35)
27		(6)		(10)	(22)	(28)	(36)
28		(7)		(13)	(23)	(30)	(39)
29		(8)		(8)		(14)	(31)	(40)


*/
