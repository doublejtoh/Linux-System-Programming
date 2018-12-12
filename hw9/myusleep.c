#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

void myusleep(int secs, int usecs) {
	struct timeval tm;
	
	tm.tv_sec = secs;
	tm.tv_usec = usecs;
	
	if (select(0, (fd_set *)0, (fd_set *)0, (fd_set *)0, &tm) < 0) {// maxfd : 0,  fd_set : 0,  1.5 seconds동안 발생하지 않을 fd로부터 ready되기를 기다림.
		perror("select");
		return;
	}
}

main() {
	printf("Sleep for 1.5 seconds.....\n");
	myusleep(1, 500000);
}
