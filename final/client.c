#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "udp.h"


main(int argc, char *argv[])
{
	int					sockfd, n, peerAddrLen;
	struct sockaddr_in	servAddr, peerAddr;
	MsgType				msg;
	char				input;
	struct	hostent		*hp;


	if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)  {
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_port = htons(SERV_UDP_PORT);


    if (isdigit(argv[1][0])) { // argv[1]의 첫byte가 숫자라면 (dotted decimal)이라면
        servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    }
    else {
        if ((hp = gethostbyname(argv[1])) == NULL) { // hostname(e.g. celinux1.khu.ac.kr)의 정보를 dns에게 요청.
            fprintf(stderr, "Unknown host: %s\n", argv[1]);
            exit(1);
        }
        memcpy(&servAddr.sin_addr, hp->h_addr, hp->h_length); // sin_addr 시작주소로부터 hp->h_length(4byte)만큼을 hp->h_addr로부터 복사 >붙여넣기.
    }


	strcpy(msg.uname, argv[2]);
	msg.is_first_req = 1;
	if (sendto(sockfd, (char *)&msg, sizeof(msg), 
			0, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)  {
		perror("sendto");
		exit(1);
	}
		
	peerAddrLen = sizeof(peerAddr);
	if ((n = recvfrom(sockfd, (char *)&msg, sizeof(msg),
				0, (struct sockaddr *)&peerAddr, &peerAddrLen)) < 0)  {
		perror("recvfrom");
		exit(1);
	}

	while(1) {
		printf("Key > ");
		input = getc(stdin);
		printf("\n");
		strcpy(msg.uname, argv[2]);
		msg.input = input;
		msg.is_first_req = 0;
    if (sendto(sockfd, (char *)&msg, sizeof(msg),
            0, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)  {
        perror("sendto");
        exit(1);
    }

		if (input == '0') {
			close(sockfd);
			exit(1);
		}
		getchar();
	}

}
