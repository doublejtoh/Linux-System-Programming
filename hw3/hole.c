#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

main() {
	int fd;
	char ch = '\0';
	 
	if ((fd = creat("file.hole", 0400)) < 0) { // file.hole이라는 파일을 owner만 Read할 수 있도록 파일을 만들어줘라.
		perror("creat");
		exit(1);
	}
	if (lseek(fd, 1023, SEEK_SET) < 0) { // lseek()는 열린 파일 지정자 flides로 부터 offset만큼 위치를 변경한다. SEEK_SET: 파일의 처음을 기준으로 offset을 계산한다. 1023개는 Null character(^@로 채워지게됨)
		perror("lseek");
		exit(1);
	}

	write(fd, &ch, 1);

	close(fd);

}
