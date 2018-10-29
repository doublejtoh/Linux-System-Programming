#include <stdio.h>

main (int argc, char * argv[], char * envp[]) {
	int i;
	char ** p;
	extern char ** environ; // 전역 변수. /etc/env 파일에 정의된 environ 전역 변수를 이 파일에서도 사용하고 싶을 때 environ을 사용.
	printf("List command-line arguments\n");
	for (i = 0; i < argc; i++) {
		printf("%s\n", argv[i]);
	}

	printf("\n");
	printf("List environment variables from environ variable\n");
#if 0
	for (i = 0; environ[i] != NULL ; i++) {
		printf("%s\n", environ[i]);
	}
#else
	for (p = environ; *p != NULL; p ++) {
		printf("%s\n", *p);
	}
#endif

	printf("\n");
	printf("List environment variables from envp variable\n");
#if 1
	for (i = 0; envp[i] != NULL; i++) {
		printf("%s\n", envp[i]);
	}
#else
	for (p = envp; *p != NULL; p++) {
		printf("%s\n", *p);
	}
#endif

}
