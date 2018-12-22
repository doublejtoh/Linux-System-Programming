#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include "udp.h"

int UID;
int	Sockfd;

void
CloseServer()
{
	close(Sockfd);
	printf("\nUDP Server exit.....\n");

	exit(0);
}


main(int argc, char *argv[])
{
	int					cliAddrLen, n;
	struct sockaddr_in	cliAddr, servAddr;
	MsgType				msg;

	UID = 0;

	signal(SIGINT, CloseServer);

	if ((Sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)  {
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(SERV_UDP_PORT);

	if (bind(Sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  {
		perror("bind");
		exit(1);
	}

	printf("UDP Server started.....\n");

	cliAddrLen = sizeof(cliAddr);
	while (1)  {
		if ((n = recvfrom(Sockfd, (char *)&msg, sizeof(msg), 
					0, (struct sockaddr *)&cliAddr, &cliAddrLen)) < 0)  {
			perror("recvfrom");
			exit(1);
		}
	
		if (msg.is_first_req == 0) {
			if (msg.input == '0') {
				printf("%s logout (UID=%d)\n", msg.uname, msg.uid);
			} else {

				printf("%d/%c\n",msg.uid,msg.input);	
			}
		}
		else if (msg.is_first_req == 1)  {
			printf("첫 접속");
			printf("%s login (UID=%d)\n", msg.uname, UID);
			msg.uid = UID;
			if (sendto(Sockfd, (char *)&msg, sizeof(msg),
                    0, (struct sockaddr *)&cliAddr, cliAddrLen) < 0)  {
            perror("sendto");
            exit(1);
        	}
			continue;
		}

	}
}
