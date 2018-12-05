#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>

#define NUM_MEN		5
#define NLOOPS		5

#define LEFT(i)		((i+NUM_MEN-1)%NUM_MEN)
#define RIGHT(i)	((i+1)%NUM_MEN)

enum { THINKING, EATING, HUNGRY }; 

sem_t	Philosopher[NUM_MEN]; // 현재 Philosopher[i]가 먹을 수 있는지 없는지에대한 여부 semaphore.
sem_t	Mutex; // Critical section 보호를 위한 Mutex Semaphore.
int		State[NUM_MEN];

void
ThreadUsleep(int usecs)
{
	pthread_cond_t		cond;
	pthread_mutex_t		mutex;
	struct timespec		ts;
	struct timeval		tv;

	if (pthread_cond_init(&cond, NULL) < 0)  { //condition value를 생성.
		perror("pthread_cond_init");
		pthread_exit(NULL);
	}
	if (pthread_mutex_init(&mutex, NULL) < 0)  { //mutex를 생성.
		perror("pthread_mutex_init");
		pthread_exit(NULL);
	}

	// 현재 시간(절대 시간)을 알아와서, 현재시간 + usec만큼의 시간을 구함.
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
	if (pthread_cond_timedwait(&cond, &mutex, &ts) < 0)  { // timedwait를 이용해서, &ts( 현재시간 + usec만큼의 절대 시간)동안 cond가 발생하기를 기다리다가 절대시간이 경과되면 wait상태에서 빠져나옴. 교수님이 cond와 mutex를 사용한 이유는 pthread_cond_timedwait를 써서 thread usleep하려고 한 것이다. cond는 여기서 절대 signal되지 않을거고, mutex또한 의미 없는데, pthread_cond_timedwait를 쓰려고 mutex와 cond를 만든것이다.
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

void
Thinking(int id)
{
	printf("Philosopher%d: Thinking.....\n", id);
	ThreadUsleep((rand()%200)*10000);
	printf("Philosopher%d: Want to eat.....\n", id);
}

void
Eating(int id)
{
	printf("Philosopher%d: Eating.....\n", id);
	ThreadUsleep((rand()%100)*10000);
}

void Test(int id) {
	if (State[id] == HUNGRY && State[LEFT(id)] != EATING && State[RIGHT(id)] != EATING) {
		State[id] = EATING;
	
		if (sem_post(&Philosopher[id]) < 0) {
			perror("sem_post");
			pthread_exit(NULL);
		}
	}
}

void PickUp(int id) {

	if (sem_wait(&Mutex) < 0) {
		perror("sem_wait");
		pthread_exit(NULL);
	}

	State[id] = HUNGRY;

	Test(id);

	if (sem_post(&Mutex) < 0) {
		perror("sem_post");
		pthread_exit(NULL);
	}

	if (sem_wait(&Philosopher[id]) < 0) {
		perror("sem_wait");
		pthread_exit(NULL);
	}
}

void PutDown(int id) {
	if (sem_wait(&Mutex) < 0) {
		perror("sem_wait");
		pthread_exit(NULL);
	}

	State[id] = THINKING;

	Test(LEFT(id)); // 내 왼쪽에 있는 애가 먹을 수 있는 상태면 signal해줌.
	Test(RIGHT(id)); // 내 오른쪽에 있는 애가 먹을 수 있는 상태면 signal해줌.

	if (sem_post(&Mutex) < 0) {
		perror("sem_post");
		pthread_exit(NULL);
	}
}

void DiningPhilosopher(int * pId) {
	int	i;
	int	id = *pId;

	for (i = 0; i < NLOOPS; i ++) {
		Thinking(id);
	
		PickUp(id);

		Eating(id);

		PutDown(id);
	}

	printf("Philosopher%d: thinking & eating %d times.....\n", id, i);

	pthread_exit(NULL);
}

main() {
	pthread_t	tid[NUM_MEN];
	int			i, id[NUM_MEN];

	srand(0x8888);

	for (i = 0; i < NUM_MEN; i++) {
		if (sem_init(&Philosopher[i], 0, 0) < 0) { // Philosohper가 먹을 수 있는 상태 semaphore배열을 다 0으로 초기화.(다 먹을 수 없다고 가정)
			perror("sem_init");
			exit(1);
		}

		id[i] = i;
	}

	if (sem_init(&Mutex, 0, 1) < 0) { // Mutex를 1로 초기화.
		perror("sem_init");
		exit(1);
	}

	for (i = 0; i < NUM_MEN; i++) {
		if (pthread_create(&tid[i], NULL, (void *) DiningPhilosopher, (void *)&id[i]) < 0) {
			perror("pthread_create");
			exit(1);
		}
	}
	
	for (i = 0;i < NUM_MEN; i++) {
		if (pthread_join(tid[i], NULL) < 0) {
			perror("pthread_join");
			exit(1);
		}
	}

	for (i = 0; i < NUM_MEN; i++) {
		if (sem_destroy(&Philosopher[i]) < 0) { // semaphore 제거
			perror("sem_destroy");
		}
	}
	
	if (sem_destroy(&Mutex) < 0) {
		perror("sem_destroy");
	}
}
