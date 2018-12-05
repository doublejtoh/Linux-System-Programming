#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include "prodcons.h"

BoundedBufferType	Buf;
pthread_cond_t		NotFull; // Condition Variable for Buf Being not full.
pthread_cond_t		NotEmpty; // Condition Variable for Buf Being not empty.
pthread_mutex_t		Mutex; // Mutex lock for Critical section protection

void ThreadUsleep(int usecs) {
	pthread_cond_t		cond;
	pthread_mutex_t		mutex;
	struct timespec		ts;
	struct timeval		tv;

	if (pthread_cond_init(&cond, NULL) < 0) { // condition variable init.
		perror("pthread_cond_init");
		pthread_exit(NULL);
	}

	if (pthread_mutex_init(&mutex, NULL) < 0) { // mutex init.
		perror("pthread_mutex_init");
		pthread_exit(NULL);
	}

	gettimeofday(&tv, NULL);
	ts.tv_sec = tv.tv_sec + usecs/1000000;
	ts.tv_nsec = (tv.tv_usec + (usecs%1000000)) * 1000;
	if (ts.tv_nsec >= 1000000000)  {
		ts.tv_nsec -= 1000000000;
		ts.tv_sec++;
	}

	if (pthread_mutex_lock(&mutex) < 0) {
		perror("pthread_mutex_lock");
		pthread_exit(NULL);
	}

	if (pthread_cond_timedwait(&cond, &mutex, &ts) < 0) {
		perror("pthread_cond_timedwait");
		pthread_exit(NULL);
	}

	if (pthread_cond_destroy(&cond) < 0) {
		perror("pthread_cond_destroy");
		pthread_exit(NULL);
	}

	if (pthread_mutex_destroy(&mutex) < 0) {
		perror("pthread_mutex_destroy");
		pthread_exit(NULL);
	}
}

void Producer(void * dummy) {
	int		i, data;
	
	printf("Producer: Start.....\n");
	
	for ( i = 0; i < NLOOPS; i++) {
		if (pthread_mutex_lock(&Mutex) < 0) { // critical section 보호를 위해서 mutex lock를 먼저 건다.
			perror("pthread_mutex_lock");
			pthread_exit(NULL);
		}

		while (Buf.counter == MAX_BUF) {
			if (pthread_cond_wait(&NotFull, &Mutex) < 0) { // if가아닌 while로 Buf.counter가 MAX_BUF인지 체크하는 이유: pthread_cond_wait에서 NotFull이 발생하기를 기다렸다가 NotFull발생한다음, 다른 producer thread가 또다시 Full로 만들었다면 이에 대해서 대비하기 위해서 while문을 쓴 것임.
				perror("pthread_cond_wait");
				pthread_exit(NULL);
			}
		}

		printf("Producer: Producing an item.....\n");
		data = (rand() % 100) * 10000;
		Buf.buf[Buf.in].data = data;
		Buf.in = (Buf.in + 1) % MAX_BUF;
		Buf.counter++;

		if (pthread_cond_signal(&NotEmpty) < 0) { // NotEmpty  cond를 기다리고 있는 특정 하나의 thread에게 signal을 보냄.
			perror("pthread_cond_signal");			
			pthread_exit(NULL);
		}
		if (pthread_mutex_unlock(&Mutex) < 0) {
			perror("pthread_mutex_unlock");
			pthread_exit(NULL);
		}

		ThreadUsleep(data);
	}

	printf("Producer : Produced %d items....\n", i);
	printf("Producer : %d items in Buffer .....\n", Buf.counter);

	pthread_exit(NULL);
}

void Consumer(void * dummy) {
	int i, data;
	
	printf("Consumer : Start.....\n");

	for ( i = 0; i < NLOOPS; i ++){
		if (pthread_mutex_lock(&Mutex) < 0) {
			perror("pthread_mutex_lock");
			pthread_exit(NULL);
		}

		while (Buf.counter == 0) {
			if (pthread_cond_wait(&NotEmpty, &Mutex) < 0) {
				perror("pthread_cond_wait");
				pthread_exit(NULL);
			}
		}

		printf("Counter: Consuming an item.....\n");
		data = Buf.buf[Buf.out].data;
		Buf.out = (Buf.out + 1) % MAX_BUF;
		Buf.counter--;

		if (pthread_cond_signal(&NotFull) < 0) {
			perror("pthread_cond_signal");
			pthread_exit(NULL);
		}

		if (pthread_mutex_unlock(&Mutex) < 0) {
			perror("pthread_mutex_unlock");
			pthread_exit(NULL);
		}

		ThreadUsleep((rand() % 100) * 10000);
	}

	printf("Consumer: Consumed %d items.....\n", i);
	printf("Consumer: %d items in buffer.....\n", Buf.counter);

	pthread_exit(NULL);
}


main() {
	pthread_t tid1, tid2;

	srand(0x8888);

	if (pthread_cond_init(&NotFull, NULL) < 0) { // Condition Variable를 생성.
		perror("pthread_cond_init");
		pthread_exit(NULL);
	}

	if (pthread_cond_init(&NotEmpty, NULL) < 0) { // condition variable 생성
		perror("pthread_cond_init");
		pthread_exit(NULL);
	}

	if (pthread_mutex_init(&Mutex, NULL) < 0) { // Mutex 생성
		perror("pthread_mutex_init");
		pthread_exit(NULL);
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

	printf("Main	: %d items in buffer.......\n", Buf.counter);

	if (pthread_cond_destroy(&NotFull) < 0) { // NotFull condition variable 삭제
		perror("pthread_cond_destroy");
		exit(1);
	}

	if (pthread_cond_destroy(&NotEmpty) < 0) {
		perror("pthread_cond_destroy");
		exit(1);
	}

	if (pthread_mutex_destroy(&Mutex) < 0) {
		perror("pthread_mutex_destroy");
		exit(1);
	}
}
