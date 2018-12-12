#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include "sg.h"


int	Sockfd;

void
CloseServer()
{
	close(Sockfd);
	printf("\nScatter/Gather TCP Server exit.....\n");

	exit(0);
}


main(int argc, char *argv[])
{
	int					newSockfd, cliAddrLen, n;
	struct sockaddr_in	cliAddr, servAddr;
	MsgType				msg;
	HeaderType			hdr;
	struct iovec		iov[2];

	signal(SIGINT, CloseServer);

	if ((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  { // tcp, ip v4 를 사용하는 소켓을 생성.
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(SERV_TCP_PORT);

	if (bind(Sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  { // serv Addr와 bind.
	 	perror("bind");
		exit(1);
	}

	listen(Sockfd, 5);

	printf("Scatter/Gather TCP Server started.....\n");

	cliAddrLen = sizeof(cliAddr);
	while (1)  {
		newSockfd = accept(Sockfd, (struct sockaddr *) &cliAddr, &cliAddrLen);
		if (newSockfd < 0)  {
			perror("accept");
			exit(1);
		}
		
		iov[0].iov_base = (char *)&hdr;
		iov[0].iov_len = sizeof(hdr);
		iov[1].iov_base = (char *)&msg;
		iov[1].iov_len = sizeof(msg);
		if ((n = readv(newSockfd, iov, 2)) < 0)  {
			perror("read");
			exit(1);
		}
		printf("Received request: %s(%s).....", msg.data, hdr.info);

		strcpy(hdr.info, "REPLY");
		msg.type = MSG_REPLY;
		sprintf(msg.data, "This is a reply from %d.", getpid());
		if (writev(newSockfd, iov, 2) < 0)  {
			perror("write");
			exit(1);
		}
		printf("Replied.\n");

		close(newSockfd);
	}
}
