#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "fifo_attend_check.h"

main () {

	char fname[MAX_FIFO_NAME];
	int fd, sfd, n;
	MsgType msg;
	
	sprintf(fname, ".fifo%d", getpid());

	if (mkfifo(fname, 0600) < 0) { // client fifo 파일 생성.
		perror("mkfifo");
		exit(1);
	}

	if ((fd = open(fname, O_RDWR)) < 0) { // client fifo 파일을 read, write할 수 있도록 open.
		perror("oepn");
		exit(1);
	}

	if ((sfd = open(SERV_FIFO, O_RDWR)) < 0) { // server fifo 파일을 read, write할 수 있도록 open.
		perror("open");
		exit(1);
	}

	strcpy(msg.returnFifo, fname); 
	
	printf("What is you name?\n");
	scanf("%127s", msg.name);
	printf("What is your Msg? Saying 'HERE' will mark you as attended.\n");
	scanf("%s127s", msg.data);
	fflush(stdin);
	write(sfd, (char *)&msg, sizeof(msg)); // 서버에게 요청.

	printf("Sent a attend check request to server.....\n");
	if ((n = read(fd, (char *)&msg, sizeof(msg))) < 0) { // 서버로부터 응답을 받음.
		perror("read");
		exit(1);
	}

	printf("Received reply from server: %s\n", msg.data);

	close(fd); // client fifo file을 닫음.
	close(sfd); // server fifo file을 닫음.
	if (remove(fname) <0) { // client fifo file을 삭제.
		perror("remove");
		exit(1);
	}
}
