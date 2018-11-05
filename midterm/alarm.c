#include <stdio.h>
#include <signal.h>

static FILE * fp; 
static int ch = 1;

void SigAlarmHandler(int signo) { // sigalarm 에 대해서 처리하는 함수
	
	char stream[1024];
	sprintf(stream, "%d\n",ch); // steram에다가 문자열을 저장.
	fputs(stream, fp); // 파일에다가 stream을 write.
	ch++;  //시험 때 못했던 부분: int자료형을 char로 cast하는 방법을 몰라서 "1\n"로 파일에 쓰이게했습니다.
}

void SigIntHandler(int singo) { // SigInt에 대해서 처리하는 함수.
	fclose(fp); // file을 close
	exit(0); // EXIT함.
}

main () {
	int ch;

	if ((fp = fopen("alarm.txt", "wt")) == NULL) { // alarm.txt를 write, text모드로 열겠다.
		perror("fopen");
		exit(1);
	}
	
	if (signal(SIGINT, SigIntHandler) == SIG_ERR) { // SIGINT에 대한 signal handler를 등록하겠다.
		perror("signal");
		exit(1);
	}

	if (signal(SIGALRM, SigAlarmHandler) == SIG_ERR) { // SIGALRM에 대한 signalHandler를 등록하겠다.
		perror("signal");
		exit(1);
	}

	ualarm(500, 500); // 500 msec 이후에 alarm signal을 보낼 것이다. 또한, 두번째 파라미터(interval)가 500 msec이기 때문에 , 500 msec마다 alarm signal을 보내게 된다.

	while(1)
		pause();

	// fputc(ch, fp);

}
