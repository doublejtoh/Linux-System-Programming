#include <stdio.h>

#define MAX_BUF 1024
main (int argc, char * argv[]){
	FILE *src, *dst;
	char buf[MAX_BUF];
	int count;
	
	if (argc != 3){
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}
	if ((src = fopen(argv[1], "rb")) == NULL) { // binary read모드로 파일을 열겠다.
		perror("fopen");
		exit(1);
		}

	if ((dst = fopen(argv[2], "wb")) == NULL) {// binary write모드로 파일을 열겠다.
		perror("fopen");
		exit(1);
	}

	while ((count = fread(buf, 1, MAX_BUF, src)) > 0 ){// fread는 1byte씩 MAX_BUF만큼 src로부터 읽어서 buf에 저장하겠다. return value는 읽어드린 item의 개수(즉, 여기서는 1byte씩 나눈 개수)를 리턴한다.
		fwrite(buf, 1, count, dst);
	}

	fclose(src);
	fclose(dst);
}
