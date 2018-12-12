#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include "semlib2.h"

char msg[100];
sem_t	saved, mutex;

void Producer (void * dummy) {
	if (sem_wait(&mutex) < 0) {
		perror("sem_wait");
		pthread_exit(NULL);
	}
	strcpy(msg, "hello");

	if (sem_post(&mutex) < 0) {
		perror("sem_post");
		pthread_exit(NULL);
	}

	if (sem_post(&saved) < 0) {
		perror("sem_post");

		pthread_exit(NULL);
	}

	pthread_exit(NULL);
}

void Consumer (void * dummy) {
	if (sem_wait(&mutex) < 0) {
		perror("sem_wait");
		pthread_exit(NULL);
	}
	if (sem_wait(&saved) < 0) {
		perror("sem_wait");
		pthread_exit(NULL);
	}

	printf("%s\n", msg);

	if (sem_post(&mutex) < 0) {
		perror("sem_post");
		pthread_exit(NULL);
	}

	pthread_exit(NULL);
}

main() {
	pthread_t tid1, tid2;
	
	if (sem_init(&saved, 0, 0) < 0) {
		perror("sem_init");
		exit(1);
	}

	if (sem_init(&mutex, 0, 1) < 0) {
		perror("sem_init");
		exit(1);	
	}
	if (pthread_create(&tid1, NULL, (void *) Producer, (void *) NULL) < 0) {
		perror("pthread_create");
		exit(1);
	}

	if (pthread_create(&tid2, NULL, (void *) Consumer, (void *) NULL) < 0) {
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
}
