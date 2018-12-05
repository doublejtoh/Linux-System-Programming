#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "semlib.h"
#include "prodcons.h"

main() {
	BoundedBufferType * pBuf;
	int		shmid, i, data;
	int 	emptySemid, fullSemid, mutexSemid;

	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0) { // shared memory를 생성 or get. (physical shared memory)
		perror("shmget");
		exit(1);
	}

	if ((pBuf = (BoundedBufferType *)shmat(shmid, 0, 0)) == (void *) -1) { // shm의 virtual address pointer를 pBuf에 저장.
		perror("shmat");
		exit(1);
	}

	if ((emptySemid = semInit(EMPTY_SEM_KEY)) < 0) { // Semaphore를 한개 생성.
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

	for ( i = 0; i < NLOOPS; i++) {
		if (semWait(emptySemid) < 0) { // emptySemid semaphore가 0이라면 wait.
			fprintf(stderr, "semWait failure\n");
			exit(1);
		}

		if (semWait(mutexSemid) < 0) {
			fprintf(stderr, "semWait failure\n");
			exit(1);	
		}

		printf("Producer: Producing an item....\n");
		data = (rand()%100)*10000;
		pBuf->buf[pBuf->in].data = data;
		pBuf->in = (pBuf->in + 1) % MAX_BUF;
		pBuf->counter++;

		if (semPost(mutexSemid) < 0) {
			fprintf(stderr, "semPost failure\n");
			exit(1);
		}

		if (semPost(fullSemid) < 0) {
			fprintf(stderr, "semPost failure\n");
			exit(1);
		}

		usleep(data);
	}

	printf("Producer: Produced &d items.....\n", i);
	
	sleep(2);

	printf("Producer: %d items in buffer.....\n", pBuf->counter);
	
	if (semDestroy(emptySemid) < 0) {
		fprintf(stderr, "semDestroy failure\n");
	}
	
	if (semDestroy(fullSemid) < 0) {
		fprintf(stderr, "semDestroy failure\n");
	}

	if (semDestroy(mutexSemid) < 0) {
		fprintf(stderr, "semDestroy failure\n");
	}

	if (shmctl(shmid, IPC_RMID, 0) < 0) {
		perror("shmclt");
		exit(1);
	}
}

/*
	질문: IPC_RMID가 어떤역할을 하는지?

*/
