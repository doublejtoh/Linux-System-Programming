#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "synclib.h"

#define NLOOPS 10
#define SIZE sizeof(long)

int update(long * ptr) {
	printf("Update 이전 ptr값: %ld\n", *ptr);
	return ((*ptr)++);
}

main() {
	int fd,i,counter;
	pid_t pid;
	caddr_t area; // character pointer type

	if ((fd = open("/dev/zero", O_RDWR)) < 0) { // /dev/zero는 special file로써, 이로부터 읽으면 무조건 0리턴, 쓰면 file이 없어짐.
		perror("open");
		exit(1);
	}

	if ((area = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == (caddr_t) -1) { // SHARED MEMORY가 생성됨. 리눅스가 설계 자체를 이렇게함./dev/zero에서 mmap하면 SHARED_MEMORY가 생성되도록.
		perror("mmap");
		exit(1);
	}

	close(fd);

	TELL_WAIT(); // Pfd1(half duplex pipe: Parent to child), Pfd2(half duplex pipe: Child to Parent) create.

	if ((pid = fork()) < 0) { // fork.
		perror("fork");
		exit(1);
	}

	else if (pid > 0) { // 부모 process
		for ( i = 0; i< NLOOPS; i += 2) {
			if (i == 0) {
				printf("i==0 일때 area값 : %ld\n", *((long *) area));
			}
			if ((counter = update((long *) area)) != i) {
				fprintf(stderr, "Counter mismatch\n");
				exit(1);
			}
			if (i == 0 ) {
				printf("i==0 일때 counter값: %ld\n", counter);
			}
			printf("Parent: counter=%d\n", counter);
			TELL_CHILD(); // Parent에서 child로 Pfd1을 통해 char 'p' write.
			WAIT_CHILD(); // Parent에서 Pfd2를 통해 char 'c'를 read하기 위해 wait.
			
		}
	}

	else {
		for (i = 1; i < NLOOPS; i += 2) {
			WAIT_PARENT(); // Child에서 Pfd1를 통해 char 'p'를 read하기 위해서 wait.
			if ((counter = update((long *) area)) != i) {
				fprintf(stderr, "Counter mismatch\n");
				exit(1);
			}
			printf("Child : counter=%d\n", counter);
			TELL_PARENT(); // Child에서 Pfd2를 통해 char 'c'를 write.
		}
	}

	

	
}


/*
질문:
area가 어떻게 업데이트 된거지?
answer: *ptr = *ptr + 1로 계속 업데이트가 된거임.

질문:
update함수에서 (*ptr)++;하면 이 counter에서 처음에 0이아니라 1이 들어가야하는거 아닌가?

return (*ptr)++; 에서 return은 그냥 (*ptr)를 해버리고, (*ptr)++는 return을 한다음에 실행되는듯. 따라서 counter에는 처음에는 0이 들어가게됨. /home/lsp41/test.c 참고.

*/
