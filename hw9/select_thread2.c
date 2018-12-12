
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include "select.h"

int		TcpSockfd; // 전역변수로 선언한 이유는 CloseServer()에서도 수행하기 위해서.
int		UdpSockfd;

void CloseServer() { // Ctrl + C 를 통해서 server를 종료했을 때 수행될 함수.
	close(TcpSockfd);
	close(UdpSockfd);

	printf("\nServer daemon exit......\n");
	exit(0);
}

void MakeTcpSocket() { // Tcp 소켓을 생성하고, bind.
	struct sockaddr_in		servAddr;
	
	if ((TcpSockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(SERV_TCP_PORT);
	
	if (bind(TcpSockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
		perror("bind");
		exit(1);
	}

	listen(TcpSockfd, 5); // 최대 5개에 대해서만 접속 허용.
}

void MakeUdpSocket() { // Udp 소켓을 생성하고, bind.
	struct	sockaddr_in		servAddr;
	
	if ((UdpSockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(SERV_UDP_PORT);

	if (bind(UdpSockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
		perror("bind");
		exit(1);
	}
}

void ProcessTcpRequest(void * dummy) { // Tcp client thread handler.
	int						newSockfd, cliAddrLen, n;
	struct 	sockaddr_in		cliAddr;
	MsgType					msg;

	cliAddrLen = sizeof(cliAddr);
	while(1) {
			newSockfd = accept(TcpSockfd, (struct sockaddr *) &cliAddr, &cliAddrLen);
			if (newSockfd < 0) {
				perror("aceept");
				pthread_exit(NULL);
			}

			if ((n = read(newSockfd, (char *)&msg, sizeof(msg))) < 0) {
				perror("read");
				pthread_exit(NULL);
			}

			printf("Received TCP request: %s.....", msg.data);

			msg.type = MSG_REPLY;
			sprintf(msg.data, "This is a reply from %d(server).", getpid());
			if (write(newSockfd, (char *)&msg, sizeof(msg)) < 0) {
				perror("write");
				pthread_exit(NULL);
			}

			printf("Replied.\n");
			close(newSockfd);
	}
	pthread_exit(NULL);
}

void ProcessUdpRequest(void * dummy) { // Udp Client thread handler.
	int						cliAddrLen, n;
	struct	sockaddr_in		cliAddr;
	MsgType					msg;
	
	cliAddrLen = sizeof(cliAddr);
	while (1) {
			if ((n = recvfrom(UdpSockfd, (char *)&msg, sizeof(msg), 0, (struct sockaddr *)&cliAddr, &cliAddrLen)) < 0) { // UdpSockfd로부터 msg 사이즈만큼 읽어서 msg에 저장.
				perror("recvfrom");
				pthread_exit(NULL);
			}

			printf("Receieved Udp request: %s.....", msg.data);
			
			msg.type = MSG_REPLY;
			sprintf(msg.data, "This is a reply from %d.", getpid());
			if (sendto(UdpSockfd, (char *)&msg, sizeof(msg), 0, (struct sockaddr *)&cliAddr, cliAddrLen) < 0) {
				perror("sendto");
				pthread_exit(NULL);
			}

			printf("Replied.\n");
	}
	pthread_exit(NULL);
}


main(int argc, char * argv[]) {
	fd_set	fdvar;
	int		count;
	pthread_t 	tcptid, udptid;

	signal(SIGINT, CloseServer); // Sigint에대한 handler 등록.
	
	MakeTcpSocket(); // TCp 소켓 생성 및 bind, listen.
	MakeUdpSocket(); // Udp 소켓 생성 및 bind.

	printf("Server daemon started....\n");
	

	if (pthread_create(&tcptid, NULL, (void *) ProcessTcpRequest, (void *) NULL) < 0) {
		perror("pthread_create");
		exit(1);
	}

	if (pthread_create(&udptid, NULL, (void *) ProcessUdpRequest, (void *) NULL) < 0) {
		perror("pthread_create");
		exit(1);
	}

	if (pthread_join(tcptid, NULL) < 0) {
		perror("pthread_join");
		exit(1);
	}
	
	if (pthread_join(udptid, NULL) < 0) {
		perror("pthread_join");
		exit(1);
	}
}

/*
select를 사용하는 이유:
select를 사용하면 Tcp, Udp 소켓 에 대해서 packet이 도착하면 이를 multiplexing해서 처리할 수 있다. 예를 들어서,

recvfrom udpsocket

accept tcpsocket

이런식으로 코드가 짜여있으면, tcpsocket에 pacekt이 오게 된다면, udpsocekt에게 data가 도착하지 않는 이상 tcpsocket에 대한 packet을 처리해줄 수 없게 된다. 따라서 이를 multiplexing해줄 수 있게끔 하는 게 select api이다.

*/
