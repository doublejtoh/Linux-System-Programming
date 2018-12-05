#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define ARRAY_SIZE	40000
#define MALLOC_SIZE	100000
#define SHM_SIZE	100000
#define SHM_MODE	(SHM_R | SHM_W)

char Array[ARRAY_SIZE];

main() {
	int shmid;
	char *ptr, *shmptr;

	if ((ptr = (char *)malloc(MALLOC_SIZE)) == NULL) { // string을  MALLOC_SIZE만큼 동적할당하겠다.
		perror("malloc");
		exit(1);
	}

	if ((shmid = shmget(IPC_PRIVATE, SHM_SIZE, SHM_MODE)) < 0) { // physical shared memory를get or create. IPC_PRIVATE를 Key로 넘겨주면, 후에 프로그램이 종료되면, shared memory를 삭제하지 않아도 된다.
		perror("shmget");
		exit(1);
	}

	if ((shmptr = shmat(shmid, 0, 0)) == (void * ) - 1) { // physical shared memory로 mapping되는 virtual address (pointer)를 get.
		perror("shmat");
		exit(1);
	}

	printf("Array[] from %x to %x\n", &Array[0], &Array[ARRAY_SIZE]); // In Data Segment.
	printf("Stack around %x\n", &shmid); // IN Stack Segment.
	printf("Malloed from %x to %x\n", ptr, ptr+MALLOC_SIZE); // In  HEAP Segment.
	printf("Shared memory attached from %x to %x\n", shmptr, shmptr+SHM_SIZE); // In Heap.

	if (shmctl(shmid, IPC_RMID, 0) < 0) { // Physical shared memory를 해제.
		perror("shmctl");
		exit(1);
	}

}


/*


Array[] from 6010a0 to 60ace0 // 6295712 ---- 4
Stack around 320f0a94 // 839846548 ---- 2
Malloed from bd4010 to bec6b0 // 12502704 ---- 3
Shared memory attached from 8cca5000 to 8ccbd6a0 // 2362068992 --- 1

*/
