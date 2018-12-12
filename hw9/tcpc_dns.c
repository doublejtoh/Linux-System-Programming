#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "tcp.h"

main(int argc, char * argv[]) {
	int						sockfd,  n;
	struct	sockaddr_in		servAddr;
	MsgType					msg;
	struct	hostent			*hp;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s Ipaddress\n", argv[0]);
		exit(1);
	}


	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) { // TCP, IP V4 소켓 생성.
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_port = htons(SERV_TCP_PORT);
	
	if (isdigit(argv[1][0])) { // argv[1]의 첫byte가 숫자라면 (dotted decimal)이라면
		servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	}
	else {
		if ((hp = gethostbyname(argv[1])) == NULL) { // hostname(e.g. celinux1.khu.ac.kr)의 정보를 dns에게 요청.
			fprintf(stderr, "Unknown host: %s\n", argv[1]);	
			exit(1);
		}
		memcpy(&servAddr.sin_addr, hp->h_addr, hp->h_length); // sin_addr 시작주소로부터 hp->h_length(4byte)만큼을 hp->h_addr로부터 복사 붙여넣기.
	}

	// bind하지 않았으므로 운영체제가 알아서 대표 ip와 사용되지 않고 있는 port하나 잡아서 bind해줌.
	if (connect(sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
		perror("connect");
		exit(1);
	}

	msg.type = MSG_REQUEST;
	sprintf(msg.data, "This is a request from %d(client).", getpid());
	if (write(sockfd, (char *)&msg, sizeof(msg)) < 0) {
		perror("write");
		exit(1);
	}
	
	printf("Sent a request.....");
	
	if ((n = read(sockfd, (char *)&msg, sizeof(msg))) < 0) {
		perror("read");
		exit(1);
	}

	printf("Received reply: %s\n", msg.data);
	close(sockfd);

}

