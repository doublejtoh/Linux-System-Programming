#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include "fifo.h"

void SigIntHandler(int signo) { // SIGINT 들어오면 SERV_FIFO FIFO파일 지우고 종료한다.
	if (remove(SERV_FIFO) < 0) {
		perror("remove");
		exit(1);
	}

	exit(0);
}

main() {
	int fd, cfd, n;
	MsgType msg;
	
	if (signal(SIGINT, SigIntHandler) == SIG_ERR) { // SIGINT에대한 핸들러 등록.
		perror("signal");
		exit(1);
	}

	if (mkfifo(SERV_FIFO, 0600) < 0) { // SERV_FIFO  FIFO 파일을 생성 하는데, 내가 생성한 process들만 write, read할 수 있는 FIFO를 만들어라. pwr------- p: pipe.
		if (errno != EEXIST) { // 이미 존재한다라는 에러뜨면 if문에 빠져들지 않음.
			perror("mkfifo");
			exit(1);
		}
	}

	if ((fd = open(SERV_FIFO, O_RDWR)) < 0) {
		perror("open");
		exit(1);
	}

	while (1) { // 계속해서 client한테 요청을 받고, client specific FIFO file로 data를 보낸다.
		if ((n = read(fd, (char *)&msg, sizeof(msg))) < 0) {
			if (errno == EINTR) { // 만약 sigchild같은게 들어오면 다시 read함.
				continue;
			} else {
				perror("read");
				exit(1);
			}
		}

		printf("Received Request: %s.....", msg.data);

		if ((cfd = open(msg.returnFifo, O_WRONLY)) < 0) {
			perror("open");
			exit(1);
		}

		sprintf(msg.data, "This is a reply from %d(server process).", getpid());
		write(cfd, (char *)&msg, sizeof(msg));
		close(cfd); // client specific fifo file을 닫음.
		printf("Replied.\n");
		
	}
}
