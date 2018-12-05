#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include "tcp.h"


int	Sockfd;

void
CloseServer()
{
	close(Sockfd);
	printf("\nTCP Server exit.....\n");

	exit(0);
}


main(int argc, char *argv[])
{
	int					newSockfd, cliAddrLen, n;
	struct sockaddr_in	cliAddr, servAddr;
	MsgType				msg;
	pid_t				pid;

	signal(SIGINT, CloseServer);

	if ((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  {
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(SERV_TCP_PORT);

	if (bind(Sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  {
		perror("bind");
		exit(1);
	}

	listen(Sockfd, 5);

	printf("TCP Server started.....\n");

	cliAddrLen = sizeof(cliAddr);
	while (1)  {
		newSockfd = accept(Sockfd, (struct sockaddr *) &cliAddr, &cliAddrLen);
		if (newSockfd < 0)  {
			perror("accept");
			exit(1);
		}

		if ((pid = fork()) < 0) {
			perror("fork");
			exit(1);
		}

		else if (pid == 0) { // 자식인 경우
			close(Sockfd); // 부모가 accept하는 사용하는 socket 먼저닫음. 필요없으니까.
			printf("Received Request: %s.....", msg.data);
			msg.type = MSG_REPLY;
			strcpy(msg.data, "This is a reply from server.");
			if (write(newSockfd, (char *)&msg, sizeof(msg)) < 0) {
				perror("write");
				exit(1);
			}
			printf("Replied.\n");
			usleep(10000);
			close(newSockfd);
			exit(0); // 이거 안해주면 child가 while문 계속 돌면서 accept(socketfd)에서 오류남. close한 sockfd를 쓰려고하니깐. 참고 url: https://stackoverflow.com/questions/37435072/bad-file-descriptor-error-on-accept
		} else { // 부모인 경우
			close(newSockfd); // newSockfd는 자식에서 사용하니깐 닫아준다.
		}
	}
}
