#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shm.h"

main() {
	int shmid;
	char * ptr, *pData;
	int * pInt;

	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0) {
		perror("shmget");
		exit(1);
	}

	if ((ptr = shmat(shmid, 0, 0)) == (void * ) - 1) {
		perror("shmat");
		exit(1);
	}

	pInt = (int *)ptr; // shared Memory의 모든 값은 처음에 0으로 initialized 되있음.
	while ((*pInt) == 0) // Busy waiting (Spin lock): cpu를 계속 소비해가면서 wait함.
		;

	// sipc2에서 *pInt에다가 1이라고 써서, while문에서 빠져나올 수 있음.
	// single cpu core라면, cpu가 번갈아가면서 sipc1, sipc2를 수행함. sipc1를 수행하다가, while문 계속 돌다가 Timer Interrupt걸리게 되서 운영체제가 수행되어 scheduling을 통해 sipc2가 수행되게 되는데, 거기서 pInt값 바꾸고, 다시 sipc1이 실행되게 되서 while문에서 벗어날 수 있음.
	// multi cpu core라면, cpu한개당 sipc1, sipc2를 각각 수행하게 되므로, sipc1이 while문 돌다가 바로 풀림.
	pData = ptr + sizeof(int); // 처음 4byte만큼은 int로 쓰고, 다음부터는 string으로 씀.
	printf("Received request: %s......", pData);
	sprintf(pData, "This is a reply from %d.", getpid());
	*pInt = 0;
	printf("Replied.\n");

	sleep(1); // 바로 shmctl해버리면 sicp2에서 shared memory를 사용하지 못하므로 sleep(1)함.
	printf("shared memory ptr : %x\n ", ptr);
	 
	if (shmctl(shmid, IPC_RMID, 0) < 0) {
		perror("shmctl");
		exit(1);
	}
}
