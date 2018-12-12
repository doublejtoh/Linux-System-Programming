#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "udp.h"

main(int argc, char * argv[]) {
	int						sockfd, n, peerAddrLen;
	struct	sockaddr_in		servAddr, peerAddr;
	MsgType					msg;

	if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR); // convert IPv4  numbers-and-dots  notation into binary data in network byte  order
	servAddr.sin_port = htons(SERV_UDP_PORT);

	msg.type = MSG_REQUEST;
	sprintf(msg.data, "This is a request from %d.", getpid());
	if (sendto(sockfd, (char *)&msg, sizeof(msg), 0, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
		perror("sendto");
		exit(1);
	}

	printf("Sent a request....");

	peerAddrLen = sizeof(peerAddr); // peerAddr로 사용하는 이유: 어디서든지 해당 client socket에게 데이터를 보낼 수 있으므로. servAddr말고 peerAddrLen을 사용. 
	if ((n = recvfrom(sockfd, (char *)&msg, sizeof(msg), 0, (struct sockaddr *)&peerAddr, &peerAddrLen)) < 0) {
		perror("recvfrom");
		exit(1);
	}

	printf("Received reply: %s\n", msg.data);
	close(sockfd);
}
