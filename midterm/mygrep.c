#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUF 256


main (int argc, char * argv[]) {
	FILE * fp;
	char buf[MAX_BUF]; // 파일에서 읽어올 buf
	int line; // line수 출력을 위한 변수

	if (argc != 3) {
		printf("Usage: %s target target_file\n", argv[0]);
		exit(1);
	}

	if ((fp = fopen(argv[2], "rt")) == NULL) { // argv[2]를 read, text 모드로 open하겠다.
		perror("fopen");
		exit(1);
	}

	line = 1;
	while (fgets(buf, MAX_BUF, fp)) { //파일에서 line by line으로 읽어오면서, buf에 저장한다.
		if (strstr(buf, argv[1])) { //만약 buf에서 argv[1]가 발견된다면, if문으로 빠진다.
			printf("%4d: %s", line, buf); //라인을 출력하고, buf를 출력한다.
		}
		line++; // line count 를 늘린다.
	}

	fclose(fp); //파일을 닫는다.

}
