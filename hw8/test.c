#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include "prodcons.h"

pthread_mutex_t 	Mutex;
pthread_cond_t		Cond;


void testThread(void * dummy) {

	if (pthread_mutex_lock(&Mutex) < 0) {
		perror("pthread_mutex_lock");
		pthread_exit(NULL);
	}

	printf("lock acquired.\n");
	if (pthread_cond_wait(&Cond, &Mutex) < 0) {
		perror("pthreaD_cond_wait");
		pthread_exit(NULL);
	}

	printf("wait freed.\n");

	if (pthread_mutex_unlock(&Mutex) < 0) {
		perror("pthread_mutex_unlock");
		pthread_exit(NULL);
	}

	pthread_exit(NULL);
}

void notifier(void * dummy) {
	
	if (pthread_cond_signal(&Cond) < 0) {
	perror("pthread_cond_signal");
			pthread_exit(NULL);
	}
}

main() {

	pthread_t 	tid1,tid2;

	
	if (pthread_cond_init(&Cond, NULL) < 0)  {
		perror("pthread_cond_init");
		pthread_exit(NULL);
	}
	if (pthread_mutex_init(&Mutex, NULL) < 0)  {
		perror("pthread_mutex_init");
		pthread_exit(NULL);
	}

	if (pthread_cond_signal(&Cond) < 0) {
	perror("pthread_cond_signal");
			pthread_exit(NULL);
	}

	if (pthread_create(&tid1,NULL, (void *) testThread, (void *) NULL) < 0) {
		perror("pthread_create");
		exit(1);
	}

	if (pthread_create(&tid2, NULL, (void *) notifier, (void *) NULL) < 0) {
		perror("pthread_create");
		exit(1);
	}

	if (pthread_cond_signal(&Cond) < 0) {
	perror("pthread_cond_signal");
			pthread_exit(NULL);
	}
	if (pthread_join(tid1, NULL) <0) {
		perror("pthread_join");
		exit(1);
	}



}
