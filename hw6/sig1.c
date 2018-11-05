#include <stdio.h>
#include <signal.h>

void SignalHandler(int signo) {
	printf("Received a SIGNINT singal\n");
	printf("Terminate this process\n"); 
	exit(0);

}


main () {

	signal(SIGINT, SignalHandler);
	printf("Press ^C to quit\n");
	while(1) {
		printf("I am working...\n");
	}
	//for ( ; ;)
	//	pause(); // signal interrupt를 기다림.
	// pause 는 process로 하여금 signal이 해당 프로세스에 전달될 때 까지 sleep하도록 함.
}
