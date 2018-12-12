#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include "fifo.h"

void
SigIntHandler(int signo)
{
	if (remove(SERV_FIFO) < 0)  {
		perror("remove");
		exit(1);
	}

	exit(0);
}


main()
{
	int			fd, cfd, n;
	MsgType		msg;

	if (signal(SIGINT, SigIntHandler) == SIG_ERR)  {
		perror("signal");
		exit(1);
	}

	if (mkfifo(SERV_FIFO, 0600) < 0)  {
		if (errno != EEXIST)  {
			perror("mkfifo");
			exit(1);
		}
	}

	if ((fd = open(SERV_FIFO, O_RDWR)) < 0)  {
		perror("open");
		exit(1);
	}

		if ((n = read(fd, (char *)&msg, sizeof(msg))) < 0)  {
			perror("read");
			exit(1);
		}
		printf("Received a message: %s\n", msg.data);

		if ((cfd = open(msg.returnFifo, O_WRONLY)) < 0)  {
			perror("open");
			exit(1);
		}
		strcpy(msg.data, "Hi");
		write(cfd, (char *)&msg, sizeof(msg));
		close(cfd);
		printf("Sent a message: %s\n", msg.data);
}
