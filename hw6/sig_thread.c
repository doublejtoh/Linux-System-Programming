#include <stdio.h>
#include <signal.h>
#include <pthread.h>

#define THREAD_MAIN

#define THREAD_1
#define THREAD_2
// signal의 특성상 signal은 특정 process를 지정해서 보내는 것이지, 특정 thread로 전달하는 것이 아니기 떄문에, process내의 어떤 thread에서 signal을 받아 처리할지 모른다. 예를들어 main thread, thread1, thread2가 잇다고 가정하면, main thread에서 signal handler를 지정하더라도, signal handler에서 처리되지 않고, thread1혹은 thread2로 signal이 전달될 수 있다.

void SigIntHandler(int signo) {
	printf("Received a SIGINT signal by thread %d\n", pthread_self());
	printf("Terminate this process\n");
		
	exit(0);
}

void Thread1(void * dummy) {
#ifdef THREAD_1
	signal(SIGINT, SigIntHandler);
#endif
	while(1)
		;

}

void Thread2(void * dummy) {
#ifdef THREAD_2
	signal(SIGINT, SigIntHandler);
#endif
	while(1)
		;
}

main () {
	pthread_t tid1, tid2;
	if (pthread_create(&tid1, NULL, Thread1, NULL) < 0) {
		perror("pthread_create");
		exit(1);
	}

	if (pthread_create(&tid2, NULL, Thread2, NULL) < 0) {
		perror("pthread_create");
		exit(1);
	}

	printf("Create two threads: tid1=%d, tid2=%d\n", tid1, tid2);
	printf("Main Thread: tid=%d\n", pthread_self());

#ifdef THREAD_MAIN
	signal(SIGINT, SigIntHandler);
#endif

	printf("Press ^C to quit\n");
	
	while(1){
		pause();
	}
}

