#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "tcp.h"

main(int argc, char * argv[]) {
	int		sockfd, n;
	struct	sockaddr_in		servAddr;
	MsgType					msg;

	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) { // socket 생성.
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR); // convert SERV_HOST_ADDR (IPv4  numbers-and-dots)  notation into binary data in network byte  order
	servAddr.sin_port = htons(SERV_TCP_PORT);

	if (connect(sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) { // connect 함으로써 server와 연결됨. 이때, connect하기 전에 bind를 하지 않아도 됨. 그냥 os가 알아서 ip 중에 하나 채택하고, port 도 빈거중에 하나 채택.
		perror("connect");
		exit(1);
	}

	msg.type = MSG_REQUEST;
	sprintf(msg.data, "This is a request from %d.", getpid());
	if (write(sockfd, (char *)&msg, sizeof(msg)) < 0) { // server에게 msg를 보냄.
		perror("write");
		exit(1);
	}

	printf("Sent a request.....");

	if ((n = read(sockfd, (char *)&msg, sizeof(msg))) < 0) {
		perror("read");
		exit(1);
	}

	printf("Received reply: %s\n", msg.data);
	close(sockfd); // read 
}
