#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>
#include "prodcons.h"

BoundedBufferType	Buf;
sem_t				EmptySem, FullSem, MutexSem;

void ThreadUsleep(int usecs) {
	pthread_cond_t		cond;
	pthread_mutex_t		mutex;
	struct timespec		ts;
	struct timeval		tv;

	if (pthread_cond_init(&cond, NULL) < 0)  {
		perror("pthread_cond_init");
		pthread_exit(NULL);
	}
	if (pthread_mutex_init(&mutex, NULL) < 0)  {
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

	if (pthread_mutex_lock(&mutex) < 0)  {
		perror("pthread_mutex_lock");
		pthread_exit(NULL);
	}
	if (pthread_cond_timedwait(&cond, &mutex, &ts) < 0)  {
		perror("pthread_cond_timedwait");
		pthread_exit(NULL);
	}

	if (pthread_cond_destroy(&cond) < 0)  {
		perror("pthread_cond_destroy");
		pthread_exit(NULL);
	}
	if (pthread_mutex_destroy(&mutex) < 0)  {
		perror("pthread_mutex_destroy");
		pthread_exit(NULL);
	}


}


void Producer (void * dummy) {

	int i, data;
	printf("Producer: Start......\n");

	for (i = 0; i < NLOOPS; i++) {
		if (sem_wait(&EmptySem) < 0) { // sem_wait는 POSIX pthread semaphore Implementation으로, EmptySem이 0인지 체크하여 0이라면 해당 thread를 wait상태로 만듬. (replacement of busy waiting.) 만약 0이아니라면 semaphore를 -1 해주고 다음 statement실행.
			perror("sem_wait");
			pthread_exit(NULL);
		}

		if (sem_wait(&MutexSem) < 0) { // MutexSem이 0인지 체크하여 0이라면 해당 thread를 wait상태로 만듬. for critical section 보호(critical section에는 하나의 스레드만 진입가능하도록 만들기위해서) 만약 0이아니라면 semaphore를 -1해주고 다음 statement 실행.
			perror("sem_wait");
			pthread_exit(NULL);
		}

		printf("Producer : Producing an item.....\n");
		data = (rand() % 100) * 10000;
		Buf.buf[Buf.in].data = data;
		Buf.in = (Buf.in + 1) % MAX_BUF;
		Buf.counter++;

		if (sem_post(&MutexSem) < 0) { // MutexSem를 wait하고 있는 thread들에게 signal 또한 semaphroe를 +1 해줌.
			perror("sem_post");
			pthread_exit(NULL);
		}
		if (sem_post(&FullSem) < 0) { // FullSem을 wait하고 있는 thread에게 signal. 또한 semaphore를 +1 해줌.
			perror("sem_post");
			pthread_exit(NULL);
		}

		ThreadUsleep(data);
	}

	printf("Producer: Produced %d items.....\n", i);
	printf("Producer: %d items in buffer.....\n", Buf.counter);
	
	pthread_exit(NULL);
}


void Consumer(void * dummy) {
	int i, data;
	printf("Consumer: Start......\n");
	
	for (i = 0 ;i < NLOOPS; i ++) {
		if (sem_wait(&FullSem) < 0) { // FullSem이 0이라면 wait. 아니라면 -1해주고 다음 statement진행.
			perror("sem_wait");
			pthread_exit(NULL);
		}

		if (sem_wait(&MutexSem) < 0) {
			perror("sem_wait");
			pthread_exit(NULL);
		}

		printf("Consumer: Consuming an item.....\n");
		data = Buf.buf[Buf.out].data;
		Buf.out = (Buf.out + 1) % MAX_BUF;
		Buf.counter--;

		if (sem_post(&MutexSem) < 0)  {
			perror("sem_post");
			pthread_exit(NULL);
		}
		if (sem_post(&EmptySem) < 0)  {
			perror("sem_post");
			pthread_exit(NULL);
		}

		ThreadUsleep((rand()%100)*10000);
	}


	printf("Consumer: Consumed %d items.....\n", i);
	printf("Consumer: %d items in buffer.....\n", Buf.counter);

	pthread_exit(NULL);

}


main()
{
	pthread_t	tid1, tid2;

	srand(0x9999);

	if (sem_init(&EmptySem, 0, MAX_BUF) < 0)  { // EmptySem의 initail value를 MAX_BUF로 초기화.
		perror("sem_init");
		exit(1);
	}
	if (sem_init(&FullSem, 0, 0) < 0)  {
		perror("sem_init");
		exit(1);
	}
	if (sem_init(&MutexSem, 0, 1) < 0)  {
		perror("sem_init");
		exit(1);
	}

	if (pthread_create(&tid1, NULL, (void *)Producer, (void *)NULL) < 0)  {
		perror("pthread_create");
		exit(1);
	}

	if (pthread_create(&tid2, NULL, (void *)Consumer, (void *)NULL) < 0)  {
		perror("pthread_create");
		exit(1);
	}

	if (pthread_join(tid1, NULL) < 0)  {
		perror("pthread_join");
		exit(1);
	}
	if (pthread_join(tid2, NULL) < 0)  {
		perror("pthread_join");
		exit(1);
	}

	printf("Main    : %d items in buffer.....\n", Buf.counter);

	if (sem_destroy(&EmptySem) < 0)  {
		perror("sem_destroy");
	}
	if (sem_destroy(&FullSem) < 0)  {
		perror("sem_destroy");
	}
	if (sem_destroy(&MutexSem) < 0)  {
		perror("sem_destroy");
	}
}
