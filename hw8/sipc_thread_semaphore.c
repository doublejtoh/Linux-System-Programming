#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "semlib2.h"

char msg[1000];
sem_t	Sipc1Sem, Sipc2Sem, MutexSem;

void sipc1(void * dummy) {
	if (sem_wait(&Sipc2Sem) < 0) { // wait for sipc2 thread to write on "msg"
		perror("sem_wait");
		pthread_exit(NULL);
	}

	if (sem_wait(&MutexSem) < 0) {
		perror("sem_wait");
		pthread_exit(NULL);
	}

	printf("SIPC1 THREAD -------- Received request: %s......", msg);
	sprintf(msg, "This is a reply from %d.", pthread_self());
	
	if (sem_post(&MutexSem) < 0) {
		perror("sem_post");
		pthread_exit(NULL);
	}

	if (sem_post(&Sipc1Sem) < 0) { // signal to sipc2 for waking up.
		perror("sem_post");
		pthread_exit(NULL);
	}

	printf("SIPC1 THREAD --------Replied.\n");
	
	pthread_exit(NULL);
}

void sipc2(void * dummy) {
	if (sem_wait(&MutexSem) < 0) { // protect critical section(msg)
		perror("sem_wait");
		pthread_exit(NULL);
	}

	sprintf(msg, "This is a request from %d.", pthread_self()); // pthread_self returns pthread_t (own thread id)

	if (sem_post(&MutexSem) < 0) { // post mutexSem.
		perror("sem_post");
		pthread_exit(NULL);
	}

	if (sem_post(&Sipc2Sem) < 0) { // tell to sipc1 thread that i wrote.
		perror("sem_post");
		pthread_exit(NULL);
	}

	if (sem_wait(&Sipc1Sem) < 0) { // wait for sipc1 to tell to sipc2.
		perror("sem_wait");
		pthread_exit(NULL);
	}

	printf("SIPC2 THREAD --------Received reply: %s\n", msg);

	pthread_exit(NULL);
}

main()
{
	pthread_t tid1, tid2;
	if (sem_init(&Sipc1Sem, 0, 0) < 0) {
		perror("sem_init");
		exit(1);
	}

	if (sem_init(&Sipc2Sem, 0, 0) < 0) {
		perror("sem_init");
		exit(1);
	}

	if (sem_init(&MutexSem, 0, 1) < 0) {
		perror("sem_init");
		exit(1);
	}

	if (pthread_create(&tid1, NULL, (void *)sipc1, (void *) NULL) < 0) {
		perror("pthread_create");
		exit(1);
	}
	
	if (pthread_create(&tid2, NULL, (void *)sipc2, (void  *) NULL) < 0) {
		perror("pthread_create");
		exit(1);
	}
	
	if (pthread_join(tid1, NULL) < 0) {
		perror("pthread_join");
		exit(1);
	}

	if (pthread_join(tid2, NULL) < 0) {
		perror("pthread_join");
		exit(1);
	}

	if (sem_destroy(&Sipc1Sem) < 0) {
		perror("sem_destroy");
	}

	if (sem_destroy(&Sipc2Sem) < 0) {
		perror("sem_destroy");
	}

	if (sem_destroy(&MutexSem) < 0) {
		perror("sem_destroy");
	}

}
