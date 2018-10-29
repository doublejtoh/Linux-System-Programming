#include <stdio.h>
#include <pthread.h>

void PrintMsg(char * msg) {
	printf("%s", msg);
	pthread_exit(NULL); // 현재 실행중인 thread를 종료. return:NULL.
}

main () {
	pthread_t tid1, tid2;
	char * msg1 = "Hello, ";
	char * msg2 = "World!\n";

	if (pthread_create(&tid1, NULL, PrintMsg, msg1) < 0) {
		perror("pthread_create");
		exit(1);
	}

	if (pthread_create(&tid2, NULL, PrintMsg, msg2) < 0) {
		perror("pthread_create");
		exit(1);
	}

	printf("Threads created: tid=%d, %d\n", tid1, tid2);

	if (pthread_join(tid1, NULL) < 0) { // thread1이 끝날 떄 까지 기다림. retval = NULL이 아니면 해당 스레드의 exit status를 copy.
		perror("pthread_join");
		exit(1);
	}

	if (pthread_join(tid2, NULL) < 0) {
		perror("pthread_join");
		exit(1);
	}

	printf("Threads terminated: tid=%d, %d\n", tid1, tid2);

}
