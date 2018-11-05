#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

main () {
	struct stat statbuf;

	if (stat("bar", &statbuf) < 0) {
		perror("stat");
		exit(1);
	}

	if (chmod("bar", (statbuf.st_mode & ~S_IWGRP) | S_ISUID) < 0 ){
		perror("chmod"); // S_IWGRP는 끄고, S_ISUID는 켜겠다
		exit(1);
	}
/*
S_ISUID, S_ISGID의 역할.
In simple words users will get file ownerâs permissions as well as owner UID and GID when executing a file/program/command.
*/
	if (chmod("foo", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0 ) {
		perror("chmod"); // S_IRUSR, S_IWUSR, S_IRGRP, S_IROTH는 켜겠다.
		exit(1);
	}

}
