#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>


#define READER_NUM 3
#define WRITER_NUM 2
#define NLOOPS 10

int readcount = 0;
sem_t	mutex;
sem_t	wrt;

void
ThreadUsleep(int usecs)
{
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

void Writer(int * pid) {
	int j;
	for (j = 0; j < NLOOPS; j++) {

	
	if (sem_wait(&wrt) < 0) {
		perror("sem_wait");
		pthread_exit(NULL);
	}

	printf("Writer %d is writing\n", *pid);

	ThreadUsleep((rand()%200)*10000);	

	if (sem_post(&wrt) < 0) {
		perror("sem_post");
		pthread_exit(NULL);
	}

	}

	pthread_exit(NULL);
}

void Reader(int * pid) {
	int j ;
	for ( j = 0 ; j < NLOOPS; j++) {
	if (sem_wait(&mutex) < 0) {
		perror("sem_wait");
		pthread_exit(NULL);
	}

	readcount++;

	if (readcount == 1) {
		if (sem_wait(&wrt) < 0) {
			perror("sem_wait");
			pthread_exit(NULL);
		}
	}

	if (sem_post(&mutex) < 0) {
		perror("sem_post");
		pthread_exit(NULL);
	}

	printf("Reader %d is reading\n", *pid);

	ThreadUsleep((rand()%100)*10000);

	if (sem_wait(&mutex) < 0) {
		perror("sem_wait");
		pthread_exit(NULL);
	}

	readcount--;
	
	if (readcount == 0) {
		if (sem_post(&wrt) < 0) {
			perror("sem_post");
			pthread_exit(NULL);
		}
	}

	if (sem_post(&mutex) < 0) {
		perror("sem_post");
		pthread_exit(NULL);
	}
	}
	pthread_exit(NULL);
}


main() {

	pthread_t	rtid[READER_NUM], wtid[WRITER_NUM];
	int i, rid[READER_NUM], wid[WRITER_NUM];
	
	srand(0x8888);
    if (sem_init(&mutex, 0, 1) < 0) { // Mutex를 1로 초기화.
        perror("sem_init");
        exit(1);
    }

    if (sem_init(&wrt, 0, 1) < 0) { // Mutex를 1로 초기화.
        perror("sem_init");
        exit(1);
    }


	for (i = 0; i < READER_NUM; i++) {
		rid[i] = i;
	}

	for (i = 0; i< WRITER_NUM; i++) {
		wid[i] = i;
	}

	for ( i = 0; i < READER_NUM; i++){
		if (pthread_create(&rtid[i], NULL,(void *) Reader, (void *)&rid[i]) < 0) {
			perror("pthread_create");
			exit(1);
		}
	}

	for ( i = 0 ;i < WRITER_NUM; i ++) {
		if (pthread_create(&wtid[i], NULL, (void *) Writer, (void *)&wid[i]) < 0) {
			perror("pthread_create");
			exit(1);
		}
	}

	for ( i =0 ; i< READER_NUM ;i++) {
        if (pthread_join(rtid[i], NULL) < 0) {
            perror("pthread_join");
            exit(1);
        }
	
	}

	for ( i =0; i < WRITER_NUM; i++) {
        if (pthread_join(wtid[i], NULL) < 0) {
            perror("pthread_join");
            exit(1);
        }

	}

	    if (sem_destroy(&mutex) < 0) {
        perror("sem_destroy");
    }


	if (sem_destroy(&wrt) < 0) {
        perror("sem_destroy");
    }
}
