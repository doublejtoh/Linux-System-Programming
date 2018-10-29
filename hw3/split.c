#include <stdio.h>
#include <unistd.h>

#define MAX_BUF 1024

long
filesize(FILE * fp) {
	long cur, size;
	cur = ftell(fp); // 현재 파일의 indicator position을 알아낸다.
	fseek(fp, 0L, SEEK_END); // 파일의 끝에서 부터 0L의 위치에 indicator position을 둔다.
	size = ftell(fp); // EOF에 파일의 indicator를 둔다.
	fseek(fp, cur, SEEK_SET); // 원래대로 indicator position을 둔다.
	return(size);

}

main(int argc, char * argv[]) {
	FILE * src, * dst1, * dst2;
	char buf[MAX_BUF];
	int count,size;
	
	if (argc != 4) {
		fprintf(stderr, "Usage: %s source dest1 dest2\n", argv[0]);
		exit(1);
	}

	if ((src = fopen(argv[1], "rb")) == NULL) {
		perror("fopen");
		exit(1);
	}

	if ((dst1 = fopen(argv[2], "wb")) == NULL) {
		perror("fopen");
		exit(1);
	}

	if ((dst2 = fopen(argv[3], "wb")) == NULL) {
		perror("fopen");
		exit(1);
	}

	size = filesize(src) / 2;
	
	while (size > 0) { // MAX_BUF만큼 계속해서 src에서 읽어서 dst1으로 쓴다.
		count = (size > MAX_BUF) ? MAX_BUF: size;
		fread(buf, 1, count, src);
		fwrite(buf, 1, count, dst1);
		size -= count;
	}
	while((count = fread(buf, 1, MAX_BUF, src)) > 0) {
		fwrite(buf, 1, count, dst2);
	}

	fclose(src);
	fclose(dst1);
	fclose(dst2);
}
