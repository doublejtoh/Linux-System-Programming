#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

// caddr_t : character address type : character에대한 pointer.
// mmap 실패시 return value: (caddr_t) -1.


main(int argc, char * argv[]) {
	int fdin, fdout;
	char *src, *dst; // caddr_t src, dst로도 해볼 것.
	//caddr_t src, dst; // 동일하게 동작.
	struct stat statbuf;
	
	if (argc != 3) {
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	if ((fdin = open(argv[1], O_RDONLY)) < 0) { // argv[1]를 open.
		perror("open");
		exit(1);
	}

	if ((fdout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0644)) < 0) { // READ, WRITE, CREATE, TRUNC: 이미 파일이 있으면 지우고 다시 create.
		perror("open");
		exit(1);
	}

	if (fstat(fdin, &statbuf) < 0) { // lstat은 filename이 들어가게 되고, fstat은 file pointer가 들어가게됨.
		perror("fstat");
		exit(1);
	}

	if (lseek(fdout, statbuf.st_size-1, SEEK_SET) < 0) { // fdout의 pointer를 앞에서부터 statbuf.st_size-1만큼 옮긴다. fdout은 create된 파일이기 때문에 파일 사이즈가 0이다. 따라서 이를 fdin과 파일 사이즈가 동일하게 하기 위해서 flseek와 write를 함.
		perror("lseek");
		exit(1);
	}

	write(fdout, "", 1); // fdout에 null character('\0') 1byte를 쓰겠다.
	
	if ((src = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fdin, 0)) == (caddr_t) -1) { // fdin file pointer에 대한 virtual address pointer를 src에 지정해주겠다. 이 때, virtual address start address는 os가 정해주도록 하고, 이 src를 통해서는 READ만 가능하도록 하고, 다른 프로세스와 SHARE가능 하도록, file pointer의 offset 0부터 mmap하겠다.
		perror("mmap");
		exit(1);
	}

	if ((dst = mmap(0, statbuf.st_size, PROT_WRITE, MAP_SHARED, fdout, 0)) == (caddr_t) -1) {
		perror("mmap");
		exit(1);
	}

	memcpy(dst, src, statbuf.st_size); // dst에서 statbuf.st_size만큼 읽어서 src에 statbuf.st_size만큼 write해라.

	close(fdin); // file pointer close
	close(fdout); // file pointer close
}
