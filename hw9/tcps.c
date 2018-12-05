#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include "tcp.h"

int Sockfd;

void CloseServer() { // Ctrl + C로 서버를 종료할 떄 handler.
	close(Sockfd); // Server socket 을 close.
	printf("\nTCP Server exit.....\n");
	exit(0);
}

main (int argc, char * argv[]) {
	int		newSockfd, cliAddrLen, n;
	struct 	sockaddr_in		cliAddr, servAddr;
	MsgType					msg;

	signal(SIGINT, CloseServer); // Interrupt signal에대한 handler 등록.

	if ((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) { // IP V4, TCP를 사용하는 socket을 하나 생성.
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr)); // servAddr 객체의 시작 부터 sizeof(servAddr) 만큼 다 byte 0 ('\0') 으로 초기화. 이 코드가 있는 이유: 먼저 0으로 다 초기화 시켜서 char sin_zero[8]를 0으로 하도록함. 나중에 port NUMBER, IP ADDRESS는 값을 변경시킬것임.

	servAddr.sin_family = PF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // servAddr.sin_addr.s_addr는 network - ordered -byte 4byte integer를 원함. 따라서 htonl(u_long param)을 통해서 INADDR_ANY(server IP중 하나를 선택하겠다)를 network -orderd -byte (Big Edian)으로 바꿔줌.
	servAddr.sin_port = htons(SERV_TCP_PORT); // htons(u_short param)을 통해서 SERV_TCP_PORT를 Big - edian 으로 바꿔줌.

	if (bind(Sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) { // servAddr 구조체를 가지고 Server socket을 bind함.
		perror("bind");
		exit(1);
	}

	listen(Sockfd, 5); // 최대 5개 client 접속 허용.

	printf("TCP Server started.....\n");

	cliAddrLen = sizeof(cliAddr);

	while(1) {
		newSockfd = accept(Sockfd, (struct sockaddr *) &cliAddr, &cliAddrLen); // client가 접속하는 순간 cliAddr에 client의 주소 정보가 저장됨. accept하는 Sockfd가 따로 있고, accept되는 순간 client와 server가 연결이 된 newSocket이 생성됨. 이걸로 client 와 server간 통신하게됨.
		if (newSockfd < 0) {
			perror("accept");
			exit(1);
		}

		if ((n = read(newSockfd, (char *)&msg, sizeof(msg))) < 0) { // read하고 write할때 struct를 가지고 데이터 read write하는게 나중에 구현할 때 편하다.
			perror("read");
			exit(1);
		}

		printf("Received request: %s.....", msg.data);
		
		msg.type = MSG_REPLY;

		sprintf(msg.data, "This is a reply from %d.", getpid());
		if (write(newSockfd, (char *)&msg, sizeof(msg)) < 0) {
			perror("write");
			exit(1);
		}

		printf("Replied.\n");

		usleep(10000); // client가 socket을 먼저 종료하도록 잠시 기다림.
		close(newSockfd); // client와 연결됬던 socket을 닫음.
	}
}
