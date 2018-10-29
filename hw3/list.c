#include <stdio.h>

#define MAX_BUF 256

main(int argc, char * argv[]) {
	FILE *fp;
	char buf[MAX_BUF];
	int line;
	
	if (argc != 2){
		printf("USAGE: %s filename\n", argv[0]);
		exit(1);
	}

	if ((fp = fopen(argv[1], "rt")) == NULL) { // text모드, read 모드로 file을 open하겠다.
		perror("fopen");
		exit(1);
	}

	line = 1;
	while (fgets(buf, MAX_BUF, fp)) {
		printf("%4d: %s", line++, buf);
	}

	fclose(fp);
}
