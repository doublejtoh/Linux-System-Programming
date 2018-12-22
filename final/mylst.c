
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>
#include <pwd.h>
#define MAX_PATH 256
main () {
	DIR * dp;
	struct dirent *dep;
	struct stat statbuf;

	char fullpath[MAX_PATH];
	if ((dp = opendir(".")) == NULL) { // directory name에 따른 directory stream을 DIR pointer로 return 해줌. 
		perror("opendir");
		exit(0);
	}

	while ( dep =  readdir(dp) ) { // directory stream pointer에 대해서 while문 을 돈다.
         if (strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0)
             continue;
		strcpy(fullpath, ".");
		strcat(fullpath, "/");
		strcat(fullpath, dep->d_name);
		if (lstat(fullpath, &statbuf) < 0) {
			perror("lstat");
			exit(1);
		}
		
		printf("%s %d\n", dep->d_name, statbuf.st_mtime); 
			
	}

	closedir(dp); // ) function closes the directory stream associated with dirp.
}



