
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "semlib.h"
#include "sipc_process_semaphore.h"

main() {
	int shmid;
	char * ptr, *pData;
	int * pInt;
	int sipc1Semid, sipc2Semid, mutexSemid;

	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0) {
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

	pInt = (int *)ptr; // shared Memory의 모든 값은 처음에 0으로 initialized 되있음.

	if (semWait(sipc2Semid) < 0) { // semWait for sipc2Sem. wait for sipc2 write.
		fprintf(stderr, "semWait failure\n");
		exit(1);
	}

	if (semWait(mutexSemid) < 0) { // sem wait for mutex lock.(critical section protection.)
		fprintf(stderr, "semWait failure\n");
		exit(1);
	}

	pData = ptr + sizeof(int); // 처음 4byte만큼은 int로 쓰고, 다음부터는 string으로 씀.
	printf("Received request: %s......", pData);
	sprintf(pData, "This is a reply from %d.", getpid());
	
	if (semPost(mutexSemid) < 0) {
		fprintf(stderr, "semPost failure\n");
		exit(1);
	}

	if (semPost(sipc1Semid) < 0) { // semPost to sipc2 that sipc1 write finished.
		fprintf(stderr, "semPost failure\n");
		exit(1);
	}

	printf("Sipc1 Replied.\n");

	sleep(1); // 바로 shmctl해버리면 sicp2에서 shared memory를 사용하지 못하므로 sleep(1)함.
	printf("shared memory ptr : %x\n ", ptr);
	 
	if (shmctl(shmid, IPC_RMID, 0) < 0) {
		perror("shmctl");
		exit(1);
	}
}
