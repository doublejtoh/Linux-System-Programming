#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void CharAtaTime(char * str) {
	char  * ptr;
	int c, i;
	setbuf(stdout, NULL); // stdout의 buffer를 NULL로 줘서 바로바로 print되도록 함. 표준출력 stream은 buffered I/O이기 때문에, 버퍼가 꽉 차기 전이나 개행 문자를 만나기 전까지는 flush하지 않는다.
	for (ptr = str;  c = *ptr++;) {
		for (i = 0; i< 999999; i++) {
			;
		}
		putc(c, stdout);
	}
}

main() {
	pid_t pid;
	if ((pid = fork()) < 0) {
		perror("fork");
		exit(1);
	}

	else if (pid ==0) {
		CharAtaTime("output from child\n");
	}
	else {
		CharAtaTime("output from parent\n");
	}
}
