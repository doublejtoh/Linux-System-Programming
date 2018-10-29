#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

main (int argc, char * argv[]) {

	struct stat statbuf;
	char * mode;
	int i;
	
	for (i = 1; i < argc; i++) {
		printf("%s: ", argv[i]);
		if (lstat(argv[i], &statbuf) < 0) { // lstat: argv[i]는 symbol link라고 가정, statbuf에다가 파일 정보를 저장.  실패하면 -1를 리턴. 성공시 0을 리턴.
			perror("lstat");
			continue;
		}
	
		if (S_ISREG(statbuf.st_mode))
			mode = "regular";
		else if (S_ISDIR(statbuf.st_mode))
			mode = "directory";
		else if (S_ISCHR(statbuf.st_mode))
			mode = "character special";
		else if (S_ISBLK(statbuf.st_mode))
			mode = "block special";
		else if (S_ISFIFO(statbuf.st_mode))
			mode = "FIFO";
		else if (S_ISLNK(statbuf.st_mode))
			mode = "symbolic link";
		else if (S_ISSOCK(statbuf.st_mode))
			mode = "socket";

		printf("%s\n", mode);
		printf("\tst_mode = %d\n", statbuf.st_mode);
		printf("\tst_ino = %d\n", statbuf.st_ino); // i-node number
		printf("\tst_dev = %d\n", statbuf.st_dev); // device id that contains file
		printf("\tst_rdev = %d\n", statbuf.st_rdev); // device id that contains file (if special file)
		printf("\tst_nlink = %d\n", statbuf.st_nlink); // number of hard links'
		printf("\tst_uid = %d\n", statbuf.st_uid); // user ID of owner
		printf("\tst_gid = %d\n", statbuf.st_gid); // group ID of owner
		printf("\tst_size = %d\n", statbuf.st_size); // total size in bytes
		printf("\tst_atime = %d\n", statbuf.st_atime); // last time of access
		printf("\tst_mtime = %d\n", statbuf.st_mtime); // last time of modify
		printf("\tst_ctime = %d\n", statbuf.st_ctime); // last time of status change.
		printf("\tst_blksize = %d\n", statbuf.st_blksize); // block size of filesystem IO
		printf("\tst_blocks = %d\n", statbuf.st_blocks); // number of 512B blocks allocated.

	}
}
