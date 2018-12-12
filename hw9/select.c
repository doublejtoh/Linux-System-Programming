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

void ProcessTcpRequest() { // Tcp client handler.
	int						newSockfd, cliAddrLen, n;
	struct 	sockaddr_in		cliAddr;
	MsgType					msg;

	cliAddrLen = sizeof(cliAddr);
	newSockfd = accept(TcpSockfd, (struct sockaddr *) &cliAddr, &cliAddrLen);
	if (newSockfd < 0) {
		perror("aceept");
		exit(1);
	}

	if ((n = read(newSockfd, (char *)&msg, sizeof(msg))) < 0) {
		perror("read");
		exit(1);
	}

	printf("Received TCP request: %s.....", msg.data);

	msg.type = MSG_REPLY;
	sprintf(msg.data, "This is a reply from %d(server).", getpid());
	if (write(newSockfd, (char *)&msg, sizeof(msg)) < 0) {
		perror("write");
		exit(1);
	}

	printf("Replied.\n");
	close(newSockfd);
}

void ProcessUdpRequest() {
	int						cliAddrLen, n;
	struct	sockaddr_in		cliAddr;
	MsgType					msg;
	
	cliAddrLen = sizeof(cliAddr);
	if ((n = recvfrom(UdpSockfd, (char *)&msg, sizeof(msg), 0, (struct sockaddr *)&cliAddr, &cliAddrLen)) < 0) { // UdpSockfd로부터 msg 사이즈만큼 읽어서 msg에 저장.
		perror("recvfrom");
		exit(1);
	}

	printf("Receieved Udp request: %s.....", msg.data);
	
	msg.type = MSG_REPLY;
	sprintf(msg.data, "This is a reply from %d.", getpid());
	if (sendto(UdpSockfd, (char *)&msg, sizeof(msg), 0, (struct sockaddr *)&cliAddr, cliAddrLen) < 0) {
		perror("sendto");
		exit(1);
	}

	printf("Replied.\n");
}


main(int argc, char * argv[]) {
	fd_set	fdvar; // synchrnous I/O multiplexing check하고 싶은 fd 리스트.
	int		count; // select의 반환값. (how many file descriptors are being ready.)
	
	signal(SIGINT, CloseServer); // Sigint에대한 handler 등록.
	
	MakeTcpSocket(); // TCp 소켓 생성 및 bind, listen.
	MakeUdpSocket(); // Udp 소켓 생성 및 bind.

	printf("Server daemon started....\n");

	while(1) {
		FD_ZERO(&fdvar); // clear all bits in fdset
		FD_SET(TcpSockfd, &fdvar); // turn the bit for TcpSockfd on.
		FD_SET(UdpSockfd, &fdvar); // turn the bit for UdpSockfd on.
		if ((count = select(10, &fdvar, (fd_set *)NULL, (fd_set *) NULL, (struct timeval *) NULL)) < 0) { // timeval NULL로 주면 영원히 기다린다는 뜻. select의 첫번째 파라미터는 검색해야될 file descriptor의 최대값. 이 10개중에 fd_var 몇개 ready되면 ready 된 file descriptor의 개수를 반환함. 마지막 파라미터는 NULL로 줬으므로 영원히 기다리겠다는 뜻.
			perror("select");
			exit(1);
		}
		
		printf("count: %d\n", count);
		while(count--) {
			if (FD_ISSET(TcpSockfd, &fdvar)) { // test the bit for TcpSockfd. TcpSockfd가 ready인지 check.
				ProcessTcpRequest();
			}

			else if (FD_ISSET(UdpSockfd, &fdvar)) {// test the bit for UdpSockfd. UdpSockfd가 ready인지 check.
				ProcessUdpRequest();
			}
		}
	}
}

/*
select를 사용하는 이유:
select를 사용하면 Tcp, Udp 소켓 에 대해서 packet이 도착하면 이를 multiplexing해서 처리할 수 있다. 예를 들어서,

recvfrom udpsocket

accept tcpsocket

이런식으로 코드가 짜여있으면, tcpsocket에 pacekt이 오게 된다면, udpsocekt에게 data가 도착하지 않는 이상 tcpsocket에 대한 packet을 처리해줄 수 없게 된다. 따라서 이를 multiplexing해줄 수 있게끔 하는 게 select api이다.

*/
