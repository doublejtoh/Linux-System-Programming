#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include "udp.h"

int Sockfd;

void CloseServer() {
	close(Sockfd);
	printf("\nUDP Server exit......\n");
	exit(0);
}

main(int argc, char * argv[]) {
	int 					cliAddrLen, n;
	struct	sockaddr_in		cliAddr, servAddr;
	MsgType					msg;
	
	signal(SIGINT, CloseServer);

	if ((Sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) { // IP V4, UDP 사용하는 소켓 생성.
		perror("socket");
	}

	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(SERV_UDP_PORT);

	if (bind(Sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
		perror("bind");
		exit(1);
	}

	printf("UDP Server started.....\n");
	// UDP는 connection less 기반이기 때문에 connect하지 않고 recvfrom, sendto를 통해서 데이터 주고 받고를 함.
	cliAddrLen = sizeof(cliAddr);
	while(1) {
		if ((n = recvfrom(Sockfd, (char *)&msg, sizeof(msg), 0, (struct sockaddr *)&cliAddr, &cliAddrLen)) < 0) {
			perror("recvfrom");
			exit(1);
		}

		printf("Received request: %s.....", msg.data);
		
		msg.type = MSG_REPLY;
		sprintf(msg.data, "This is a reply from %d.", getpid());
		if (sendto(Sockfd, (char *)&msg, sizeof(msg), 0, (struct sockaddr *)&cliAddr, cliAddrLen) < 0) {
			perror("sendto");
			exit(1);
		}

		printf("Replied.\n");
	}
}
