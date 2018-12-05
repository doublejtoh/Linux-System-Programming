#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "sipc_process_semaphore.h"

main() {
	int shmid;
	char *ptr, *pData;
	int *pInt;
	int sipc1Semid, sipc2Semid, mutexSemid;	

	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0) { // get or create Shared memory.
		perror("shmget");
		exit(1);
	}

	if ((ptr = shmat(shmid, 0, 0)) == (void * ) - 1) {
		perror("shmat");
		exit(1);
	}

	if ((sipc1Semid = semInit(SIPC1_SEM_KEY)) < 0) {
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}

	if ((sipc2Semid = semInit(SIPC2_SEM_KEY)) < 0) {
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}

	if ((mutexSemid = semInit(MUTEX_SEM_KEY)) < 0) {
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}

	if (semInitValue(mutexSemid, 1) < 0) {
		fprintf(stderr, "semInitValue failure\n");
		exit(1);
	}

	if (semWait(mutexSemid) < 0) {
		fprintf(stderr, "semWait failure\n");
		exit(1);
	}

	pInt = (int *) ptr;
	pData = ptr + sizeof(int);
	sprintf(pData, "This is a request from %d.", getpid());
	
	if (semPost(mutexSemid) < 0) {
		fprintf(stderr, "semPost failure\n");
		exit(1);
	}

	if (semPost(sipc2Semid) < 0) { // sipc1에게 나 다 썼다고 알려줌.
		fprintf(stderr, "semPost failure\n");
		exit(1);
	}

	printf("Sent a request.......");


	if (semWait(sipc1Semid) < 0) {
		fprintf(stderr, "semWait failure\n");
		exit(1);
	}

	printf("Received reply: %s\n", pData);
	printf("ptr: %x\n", ptr);
}

