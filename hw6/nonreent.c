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
		} else {
			printf("Return value correct.\n");
		}
	
	}
/* 
질문. 
gdb 환경에서 start 후, next로 확인해보면
36라인에서 strcmp되다가 signal handler 수행되서 ptr의 pw_name이 root로 바뀌게 되서 36라인의 if문으로 들어가게 되는 것을 확인할 수 있으나, 실행파일로 실행해보면 이를 확인할 수 없는데, 이 이유가 궁금하다.

유닉스에서는 getpwnam의 static struct passwd data;를 건드리는 부분이 critical section인데도 그냥 lock걸고 unlock하는 로직을 안씀. 따라서 36라인의 strcmp에서 printf("Retur value corrupted"에 빠지게되는데 반면에 리눅스에서는 lock걸고 unlock하는 로직이 있으므로 main thread가 getpwnam하다가 critical section들어가서 lock 하고 그다음에 바로 signal hanlder 수행되면, signal handler에서는 critical section이 main thread에 의해서 lock되어있으므로 signal handler에서 해당 critial section 못들어감. 이때, signal handler가 종료되야 main thread가 lock을 푸는데, 그러지를 못하므로 deadlock 발생.
*/
}
