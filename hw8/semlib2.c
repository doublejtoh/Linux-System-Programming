#include <stdio.h>
#include <pthread.h>
#include "semlib2.h"


// thread간 semaphore(critical section) logic을 pthread_mutex, pthread_cond가지고  직접 구현.
int sem_init(sem_t * sem, int pshared, int value) {
	if (pshared) { // pshared는 프로세스간 sem init할 떄 쓰이는 옵션인데 현재 POSIX library들은 전부 이걸 지원 안하기 떄문에 pshared는 무조건 0으로 한다 .
		fprintf(stderr, "Function not supported\n");
		return -1;
	}

	if (pthread_mutex_init(&sem->mutex, NULL) < 0) { // Mutex 생성.
		return -1;
	}

	if (pthread_cond_init(&sem->cond, NULL) < 0) { // Cond Var 생성.
		return -1;
	}

	sem->sval = value; // semaphore 초기값 설정.

	return 0;
}

int sem_wait(sem_t * sem) {
	if (pthread_mutex_lock(&sem->mutex) < 0) { // Semaphore(critical section)을 지키기 위해서 mutex를 lock함.
		return -1;
	}

	if (sem->sval == 0) {
		while (sem->sval == 0) {
			if (pthread_cond_wait(&sem->cond, &sem->mutex) < 0) {
				if (pthread_mutex_unlock(&sem->mutex) < 0) { // cond_wait에서 sem->mutex를 잠시 풀어주는데, 만약 에러떄문에 sem->mutex를 잠시 풀어주지 않았다면, 확실하게 풀어주겠다.
					return -1;
				}
				return -1;
			}
		}
		sem->sval--;
	}
	else {
		sem->sval--;
	}

	if (pthread_mutex_unlock(&sem->mutex) < 0) {
		return -1;
	}

	return 0;
}

int sem_trywait(sem_t * sem) {
	if (pthread_mutex_lock(&sem->mutex) < 0) {
		return -1;
	}

	if (sem->sval == 0 ){ // 0이라면 일단 mutex unlock하고, 그냥 return -1 해버린다.
		if (pthread_mutex_unlock(&sem->mutex) < 0) { // 
			return -1;
		}

		return -1;
	}

	else {
		sem->sval--;
	}

	if (pthread_mutex_unlock(&sem->mutex) < 0) {
		return -1;
	}

	return 0;
}

int sem_post(sem_t * sem) {
	if (pthread_mutex_lock(&sem->mutex) < 0) {
		return -1;
	}

	if (sem->sval == 0) { // semaphore 현재 값이 0이라면 signal해서 sem_wait에게  알려주겠다.
		if (pthread_cond_signal(&sem->cond) < 0) {
			if (pthread_mutex_unlock(&sem->mutex) < 0) {
				return -1;
			}

			return -1;
		}
	}

	sem->sval++;

	if (pthread_mutex_unlock(&sem->mutex) < 0) {
		return -1;	
	}

	return 0;
}

int sem_getvalue(sem_t * sem, int * sval) {
	*sval = sem->sval;

	return 0;
}

int sem_destroy(sem_t * sem) {
	if (pthread_mutex_destroy(&sem->mutex) < 0) {
		return -1;
	}

	if (pthread_cond_destroy(&sem->cond) < 0) {
		return -1;
	}

	return 0;
}
