#include <pthread.h>

// semaphore를 mutex, cond variable로 구현.

typedef struct {
	int sval;  // semaphore의 값.
	pthread_mutex_t mutex; // semaphore(critical section)를 보호하기 위한 mutex.
	pthread_cond_t	cond; // semaphore를 wait, signal하기 위한 condition variable.
} sem_t;

int 	sem_init(sem_t * sem, int pshared, int value);
int		sem_wait(sem_t * sem);
int		sem_trywait(sem_t * sem);
int		sem_post(sem_t * sem);
int		sem_getvalue(sem_t * sem, int * sval);
int		sem_destroy(sem_t * sem);
