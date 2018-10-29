#include <stdio.h>
#include <signal.h>

static unsigned int AlaramSecs;

void SigAlarmHandler(int signo) {

	/* 이부분이 없어도 똑같이 독장하기는 한다.*/ 
	if (signal(signo, SigAlarmHandler) == SIG_ERR) {
		perror("signal");
		exit(1);
	}
	

	alarm(AlaramSecs);

	printf(".");
	fflush(stdout); // "\n"이 없더라도 buffer cache에 있는 걸 stdout에 commit하도록 함. 계속 ...이 찍히도록 하기 위함.
	return;
}

int SetPeriodicAlarm(unsigned int nsecs) {

	if (signal(SIGALRM, SigAlarmHandler) == SIG_ERR) {
		return -1;
	}

	AlaramSecs = nsecs;

	alarm(nsecs);

	return 0;


}

main () {

	printf("Doing something every one seconds\n");
	
	SetPeriodicAlarm(1);
	
	while(1) {
		pause();
	}
}
