#include <stdio.h>
#include <signal.h>

void SigAlarmHandler(int signo) {
	return;
}

unsigned int mysleep(unsigned int nsecs) {
	if (signal(SIGALRM, SigAlarmHandler) == SIG_ERR) {
		return nsecs;
	}

	alarm(nsecs);

	pause();

	return  alarm(0); // any pending alarm is canceled.
	// alarm returns the number of seconds remaining until any previously scheduled alarm was due to be delivered, or zero if there was no previously scheduled alarm.

}

main () {
	int sec;
	int requested_sec = 5;
	sec = mysleep(requested_sec);
	

	printf("you set %d sec alarm and its remained time was %d\n", requested_sec, sec);

}
