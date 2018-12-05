#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "fifo.h"

main() {
	char fname[MAX_FIFO_NAME];
	int fd, sfd, n;
	MsgType msg;
	
	sprintf(fname, ".fifo%d", getpid());
	
	if (mkfifo(fname, 0600) < 0) { // client specific fifo file을 생성
		perror("mkfifo");
		exit(1);
	}

	if ((fd = open(fname, O_RDWR)) < 0) { // client specific fifo file을 open
		perror("open");
		exit(1);
	}

	if ((sfd = open(SERV_FIFO, O_RDWR)) < 0) { // well known  fifo file(for requests of clients)을 open.
		perror("open");
		exit(1);
	}

	strcpy(msg.returnFifo, fname);
	sprintf(msg.data, "This is a request from %d(client process).", getpid());
	write(sfd, (char *)&msg, sizeof(msg));
	printf("Sent a request.....");
	if (( n = read(fd, (char *)&msg, sizeof(msg))) < 0) { // request 보낸 후, client가 server의 reply를  기다림.
		perror("read");
		exit(1);
	}

	printf("Received reply: %s\n", msg.data);

	close(fd);
	close(sfd);
	if (remove(fname) < 0) {
		perror("remove");
		exit(1);
	}


}
