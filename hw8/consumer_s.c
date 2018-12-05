#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "semlib.h"
#include "prodcons.h"

main() {
	BoundedBufferType *pBuf;
	int		shmid, i ,data;
	int		emptySemid, fullSemid, mutexSemid;

	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0) { // shm를 get or create.
		perror("shmget");
		exit(1);
	}

	if ((pBuf = (BoundedBufferType *) shmat(shmid, 0, 0)) == (void *) -1) { // shm를 가르키는 pointer 를 pBuf에 저장.
		perror("shmat");
		exit(1);
	}

	if ((emptySemid = semInit(EMPTY_SEM_KEY)) < 0) {
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}

	if ((fullSemid = semInit(FULL_SEM_KEY)) < 0) {
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}

	if ((mutexSemid = semInit(MUTEX_SEM_KEY)) < 0) {
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}

	if (semInitValue(emptySemid, MAX_BUF) < 0) { // emptySemid semaphore를 MAX_BUF 로 값을 initialize. producer_s process에서도 emptySemid가 MAX_BUF로 바뀌어서, producer_s에서 emptySemid를 wait하다가 먼저 풀림.
		fprintf(stderr, "semInitValue failure\n");
		exit(1);
	}

	if (semInitValue(fullSemid, 0) < 0) { // producer_s process가 수행중이라면 그 프로세스에서도 fullSemid의 값이 0으로 바뀜.
		fprintf(stderr, "semInitValue failure\n");
		exit(1);
	}

	if (semInitValue(mutexSemid, 1) < 0) { // producer_S process에서도 
		fprintf(stderr, "semInitValue failure\n");
		exit(1);
	}
	
	srand(0x9999);

	for ( i = 0 ; i < NLOOPS; i++) {
		if (semWait(fullSemid) < 0) { // producer process가먼저 수행됨. fullSemid는 0으로 값을 바꿔주었으니.
			fprintf(stderr, "semWait failure\n");
			exit(1);
		}

		if (semWait(mutexSemid) < 0) {
			fprintf(stderr, "semWait failure\n");
			exit(1);
		}

		printf("Consumer: Consuming an item.....\n");
		data = pBuf->buf[pBuf->out].data;
		pBuf->out = (pBuf->out + 1) % MAX_BUF;
		pBuf->counter--;

		if (semPost(mutexSemid) < 0) { // mutex lock을 풀어줌.
			fprintf(stderr, "semPost failure\n");
			exit(1);
		}

		if (semPost(emptySemid) < 0) { // emptySem 값을 1증가시킴.
			fprintf(stderr, "semPost failure\n");
			exit(1);
		}

		usleep((rand() % 100) * 10000);
	
		
	}

	printf("Consumer: Consumed %d items....\n", i);
	printf("Consumer: %d items in buffer.....\n", pBuf->counter);
}

// Producer_s에서 semDestroy해줬으니 여기서는 해주면안된다.
