#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

main () {
	DIR * dp;
	struct dirent *dep;

	if ((dp = opendir(".")) == NULL) { // directory name에 따른 directory stream을 DIR pointer로 return 해줌. 
		perror("opendir");
		exit(0);
	}

	while ( dep =  readdir(dp) ) { // directory stream pointer에 대해서 while문 을 돈다.
		printf("%s\n", dep->d_name);
		
	}

	closedir(dp); // ) function closes the directory stream associated with dirp.
}
