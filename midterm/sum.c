#include <stdio.h>
#include <stdlib.h>
int sum(int a, int b) {
	
	return a + b;

}


main (int argc, char * argv[]) {
	
	if (argc != 3) {
		fprintf(stderr, "Usage: %s a b \n", argv[0]);
		exit(1);
	}

	int a = atoi(argv[1]); // atoi 함수는 들어오는 char * 형 파라미터를 int형 자료형 으로 바꾸어준다.
	int b = atoi(argv[2]);
	printf("%d+%d=%d\n",a,b,sum(a,b));

}
