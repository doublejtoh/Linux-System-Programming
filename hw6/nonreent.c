#include <stdio.h>
#include <signal.h>
#include <pwd.h>

void MyAlarmHandler(int signo) {
	struct passwd * rootptr;

	signal(SIGALRM, MyAlarmHandler); // 1초마다 계속 alarm을 signal 보내고, 이에 대해서 MyAlarmHandler가 계속 처리함.

	alarm(1);

	printf("in signal handler\n");

	if ((rootptr = getpwnam("root")) == NULL) { // "root"사용자의 passwd객체의 주소를 리턴해주는 함수 getpwnam.
		perror("getpwnam");
		exit(1);
	} else {
		printf("%s\n", rootptr->pw_name);
	}
	printf("return \n");
	return;
}

main() {
	struct passwd * ptr;

	signal(SIGALRM, MyAlarmHandler);
	alarm(1);

	for ( ; ;)  {
		if ((ptr = getpwnam("lsp41")) == NULL) {
			perror("getpwnam");
			exit(1);
		}

		if (strcmp(ptr->pw_name, "lsp41") != 0) { // 어느 순간 signal alarm이 실행되서 getpwnam("root")가 실행되면, static 변수(pwd)의 값을 바꾸고, pwd의 주소를 return하게 되고, 여기서 ptr은 static 변수의 주소를 값으로 저장하게 됨. 따라서 ptr->pw_name 는 "root"가 되므로 이 줄에서 strcmp 리턴 값이 0이 아니게됨. 
			printf("Return value corrupted!, pw_name = %s\n", ptr->pw_name);
			exit(0);
		}
	
	}
/* 
질문. 
gdb 환경에서 start 후, next로 확인해보면
36라인에서 strcmp되다가 signal handler 수행되서 ptr의 pw_name이 root로 바뀌게 되서 36라인의 if문으로 들어가게 되는 것을 확인할 수 있으나, 실행파일로 실행해보면 이를 확인할 수 없는데, 이 이유가 궁금하다.
*/
}
