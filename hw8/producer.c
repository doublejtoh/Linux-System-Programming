#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "prodcons.h"

main() {
	BoundedBufferType *pBuf;
	int shmid, i, data;

	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0) { // Physical memory를 get or create.
		perror("shmget");
		exit(1);
	}

	if ((pBuf = (BoundedBufferType *)shmat(shmid, 0, 0)) == (void *) -1) { //physical shared  memory에 mapping되는 pointer를 pBuf에 저장.
		perror("shmat");
		exit(1);
	}

	srand(0x8888);

	printf("Initial Buf in: %d , out : %d, counter: %d, data : %d, %d\n", pBuf->in, pBuf->out, pBuf->counter, pBuf->buf[0].data, pBuf->buf[1].data); 
	for (i = 0; i < NLOOPS; i++) {
		if (pBuf->counter == MAX_BUF) {
			printf("Producer: Buffer full. Waiting......\n");
			while (pBuf->counter == MAX_BUF)
				;
		}

		printf("Producer: Producing an item.....\n");
		data = (rand() % 100) * 10000;
		pBuf->buf[pBuf->in].data = data;
		pBuf->in = (pBuf->in + 1) % MAX_BUF;
		pBuf->counter++;

		usleep(data);
	}

	printf("Producer : Produced %d items....\n", i);

	sleep(2);
	printf("Producer : %d items in buffer......\n", pBuf->counter);

	if (shmctl(shmid, IPC_RMID, 0) < 0) { // physical shared memory를 destroy
		perror("shmctl");
		exit(1);
	}

}
