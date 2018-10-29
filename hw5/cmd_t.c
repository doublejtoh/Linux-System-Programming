#include <stdio.h>
#include <pthread.h>

#define MAX_CMD 256

void DoCmd(char * cmd) {
	printf("Doing %s", cmd);
	sleep(1);
	printf("Done \n");
	pthread_exit(NULL);

}

main () {
	pthread_t tid;
	char cmd[MAX_CMD];

	while (1) {
		printf("CMD > ");
		fgets(cmd, MAX_CMD, stdin);
		if (cmd[0] == 'q')
			break;
		if (pthread_create(&tid, NULL, DoCmd, cmd) < 0) {
			perror("pthread_create");
			exit(1);
		}
#if 0
		pthread_join(tid, NULL);
		
#endif
	}

}
