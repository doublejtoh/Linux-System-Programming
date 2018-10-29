#include <stdio.h>
#include <pthread.h>
#include <signal.h>

pthread_t ThreadId[2];

void SigIntHandler(int signo) {
	int i;
	printf("Received a SIGINT signal by thread %d\n", pthread_self());
	printf("Terminate other threads: tid=%d, %d\n", ThreadId[0], ThreadId[1]);
	for (i = 0; i < 2; i++) {
		if (pthread_cancel(ThreadId[i]) != 0) {
			perror("pthread_cancel");
			exit(1);
		} 
		/* pthread cancel request가 요청되면,

		1. cancellation clean up handler가 수행됨.
		2. Thread specific data destructor가 수행됨.
		3. pthread_exit(3).

		*/
	}

	for (i = 0; i < 2; i++) {
		if (pthread_join(ThreadId[i], NULL)) {
			perror("pthread_join");
			exit(1);
		}
	}

	printf("Threads Terminated: tid=%d, %d\n", ThreadId[0], ThreadId[1]);

	exit(0);

}

void Thread1(void * dummy) {
	printf("Thread %d created.....\n", pthread_self());
	
	if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL)) { // disable로 주면 disable request가 queue로 있다가 pthread가 cancellation을 허락하면 수행이 됨.  enable로 하면 setcancelstate가 asynchronous이면 바로 종료된다.(대부분). deferred라면 cancellation point를 지난 다음에 까지 cancel이 지연됬다가 그다음에야 cancel를 할 수 있게 된다.

		perror("pthread_setcancelstate");
		pthread_exit(NULL);
	
	}

	if (pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL)) {
		perror("pthread_setcanceltype");
		pthread_exit(NULL);
	}

	while(1)
		;
}

void Thread2(void * dummy) {
	printf("Thread %d created....\n", pthread_self());

	if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL)) { // 두번쨰 파라미터에는 oldstate의 int pointer변수가 들어가있다.

		perror("pthread_cancelstate");
		pthread_exit(NULL);
	}
	
	if (pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL)) {
		perror("pthread_canceltype");
		pthread_exit(NULL);
	}

	while(1)
		;
}

main() {
	if (pthread_create(&ThreadId[0], NULL, Thread1, NULL) < 0) {
		perror("pthread_create");
		exit(1);
	}
	
	if (pthread_create(&ThreadId[1], NULL, Thread2, NULL) < 0) {
		perror("pthread_create");
		exit(1);
	}

	signal(SIGINT, SigIntHandler);

	printf("Press ^C to quit all Threads.\n");

	for ( ; ;)
		pause();
}
