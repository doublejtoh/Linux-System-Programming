#include <stdio.h>
#include <pthread.h>
#include "semlib2.h"


int
sem_init(sem_t *sem, int pshared, int value)
{
	if (pshared)  {
		fprintf(stderr, "Function not supported\n");
		return -1;
	}

	if (pthread_mutex_init(&sem->mutex, NULL) < 0)
		return -1;

	if (pthread_cond_init(&sem->cond, NULL) < 0)
		return -1;

	sem->sval = value;

	return 0;
}

int
sem_wait(sem_t *sem)
{
	if (pthread_mutex_lock(&sem->mutex) < 0)
		return -1;

	if (sem->sval == 0)  {
		while (sem->sval == 0)  {
			if (pthread_cond_wait(&sem->cond, &sem->mutex) < 0)  {
				if (pthread_mutex_unlock(&sem->mutex) < 0)
					return -1;
				return -1;
			}
		}
		sem->sval--;
	}
	else  {
		sem->sval--;
	}

	if (pthread_mutex_unlock(&sem->mutex) < 0)
		return -1;

	return 0;
}

int
sem_trywait(sem_t *sem)
{
	if (pthread_mutex_lock(&sem->mutex) < 0)
		return -1;

	if (sem->sval == 0)  {
		if (pthread_mutex_unlock(&sem->mutex) < 0)
			return -1;
		return -1;
	}
	else  {
		sem->sval--;
	}

	if (pthread_mutex_unlock(&sem->mutex) < 0)
		return -1;

	return 0;
}

int
sem_post(sem_t *sem)
{
	if (pthread_mutex_lock(&sem->mutex) < 0)
		return -1;

	if (sem->sval == 0)  {
		if (pthread_cond_signal(&sem->cond) < 0)  {
			if (pthread_mutex_unlock(&sem->mutex) < 0)
				return -1;
			return -1;
		}
	}

	sem->sval++;

	if (pthread_mutex_unlock(&sem->mutex) < 0)
		return -1;

	return 0;
}

int
sem_getvalue(sem_t *sem, int *sval)
{
	*sval = sem->sval;

	return 0;
}

int
sem_destroy(sem_t *sem)
{
	if (pthread_mutex_destroy(&sem->mutex) < 0)
		return -1;

	if (pthread_cond_destroy(&sem->cond) < 0)
		return -1;

	return 0;
}

