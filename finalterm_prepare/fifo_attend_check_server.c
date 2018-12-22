#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include "fifo_attend_check.h"
#define MAX_STUDENTS 3
Attend attend[MAX_STUDENTS]; //각 학생에 대한 출석부.
int access_count; // server에 접속 시도한 학생 수.

void SigIntHandler(int signo) { // 서버가 종료될 때 이 함수가 실행된다.	
	int i;
	FILE * dst;
	char stream[1024];
	char attended;

	if ((dst = fopen(ATTENDANCE_FILE, "wt")) == NULL) {
		perror("fopen");
		exit(1);
	}

	// ATTENDANCE_FILE에다가 출석 현황을 기록.
	for (i = 0; i < access_count; i++) {
		if (attend[i].isAttend == 0) {
			attended = 'X';
		}
		else if (attend[i].isAttend == 1) {
			attended = 'O';
		}		
		sprintf(stream, "%s %c\n", attend[i].name, attended);
		fputs(stream, dst);			
	}
	
	printf("Attendance File recorded.\n");

	if (remove(SERV_FIFO) < 0) { // SERV_FIFO (FIFO)파일을 지운다.
		perror("remove");
		exit(1);
	}

	printf("Server Fifo File removed.\n");

	printf("Server Exit......\n");
	exit(0);
}

main() {

	int fd, cfd, n;
	MsgType msg;
	//attend[MAX_STUDENTS] = {0}; // 각 student에 대한 출석부 Class들 초기화.
	
	access_count = 0; // 현재 몇명이 server에게 요청하였는지에 대한 count값.
	if (signal(SIGINT, SigIntHandler) == SIG_ERR) { // SigIntHandler 등록.
		perror("signal");
		exit(1);
	}

	if (mkfifo(SERV_FIFO, 0600) < 0) { // SERV_FIFO FIFO파일을 하나 생성.
		if (errno != EEXIST) {
			perror("mkfifo");
			exit(1);
		}
	}
	
	if ((fd = open(SERV_FIFO, O_RDWR)) < 0) { // SERV_FIFO 파일을 READ 및 WRITE가능하도록 open.
		perror("oepn");
		exit(1);
	}


	while (1) {
		if (( n = read(fd, (char *)&msg, sizeof(msg))) < 0) { // SERV_FIFO파일에서 msg객체를 한개를 읽어옴.
			if (errno == EINTR) { // 만약 SigInt걸려서 read에서의 blocking이  풀린거라면, 다시 read 하도록함.
				continue;
			}

			else { // 다른 signal이 들어와서 read에서의 blocking이 풀린거라면, 에러메시지 송출. 
				perror("read");
				exit(1);
			}
		}

			if ((cfd = open(msg.returnFifo, O_WRONLY)) < 0) { // client에게 reply해줄 fifo File을 write만 할 수있게 open함.
				perror("open");
				exit(1);
			}
		
		strcpy(attend[access_count].name, msg.name);
		if (!strcmp(msg.data, "HERE")) { //만약 client가 보내온 msg.data가 "HERE"이면
			attend[access_count].isAttend = 1; // Mark as Attended라고 해준다.

			strcpy(msg.data, "You are marked as Attended.");

			write(cfd, (char *)&msg, sizeof(msg)); // client에게 reply를 보낸다.
			close(cfd);
		} else { // 만약 client가 보내온 msg.data가 "HERE"이 아니라면,
			attend[access_count].isAttend = 0; // Mark as Not attended.
			strcpy(msg.data, "you are not marked as Attended.");
			write(cfd, (char *)&msg, sizeof(msg));
			close(cfd);
		}
		
		printf("Replied to student %s. \n", msg.name);

		access_count++; //server 접속 시도 횟수 +1
		
		if (access_count == MAX_STUDENTS) { // server 접속 시도한 client 수가 MAX_STUDENTS와 같아진다면, server를 종료.
			printf("Student access count reached Maximum : %d\n", MAX_STUDENTS);
			SigIntHandler(0);
		}
	}
}
