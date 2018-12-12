#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include "fifo_attend_check.h"
#define MAX_STUDENTS 3

void SigIntHandler(int signo) {
	
	if (remove(SERV_FIFO) < 0) {
		perror("remove");
		exit(1);
	}

	exit(0);
}

main() {

	int fd, cfd, n;
	MsgType msg;
	Attend attend[MAX_STUDENTS] = {0}; // 0으로 다 초기화.
	int count = 0;
	if (signal(SIGINT, SigIntHandler) == SIG_ERR) {
		perror("signal");
		exit(1);
	}

	if (mkfifo(SERV_FIFO, 0600) < 0) {
		if (errno != EEXIST) {
			perror("mkfifo");
			exit(1);
		}
	}
	
	if ((fd = open(SERV_FIFO, O_RDWR)) < 0) {
		perror("oepn");
		exit(1);
	}


	while (1) {
		if (( n = read(fd, (char *)&msg, sizeof(msg))) < 0) {
			if (errno == EINTR) {
				continue;
			}

			else {
				perror("read");
				exit(1);
			}
		}

			if ((cfd = open(msg.returnFifo, O_WRONLY)) < 0) {
				perror("open");
				exit(1);
			}

		if (!strcmp(msg.data, "HERE")) {
			strcpy(attend[count].name, msg.name);
			attend[count].isAttend = 1;
			count++;

			strcpy(msg.data, "You are marked as Attended.");

			write(cfd, (char *)&msg, sizeof(msg));
			close(cfd);

			printf("Replied to student %s.\n", msg.name);
		} else {
			strcpy(msg.data, "you are not marked as Attended.");
			write(cfd, (char *)&msg, sizeof(msg));
			close(cfd);
			printf("Replied to student %s. \n", msg.name);
		}
		
		if (count == MAX_STUDENTS) {
			SigIntHandler(0);
		}
	}
}
