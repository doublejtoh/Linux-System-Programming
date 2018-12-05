#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

long filesize(FILE *fp) {
	long cur,size;
	
	cur = ftell(fp);

	fseek(fp, 0L, SEEK_END);
	size = ftell(fp);
	fseek(fp, cur, SEEK_SET);

	return size;

}

main() {
	DIR * dp;
	struct dirent * dep;
	struct stat statbuf;
	char fullpath[256];
	FILE * src;
	if ((dp = opendir("./")) == NULL) {
		perror("opendir");
		exit(0);
	}

	while (dep = readdir(dp)) {
		if (strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0)
			continue;
		strcpy(fullpath, "./");
		strcat(fullpath, dep->d_name);
		if (stat(fullpath, &statbuf) < 0) {
			perror("stat");
			exit(1);
		}

		if (S_ISDIR(statbuf.st_mode)) {

		} else {
			if ((src = fopen(fullpath, "r")) == NULL) {
				perror("fopen");
				exit(1);
			}

			if (statbuf.st_size < 500) {
				printf("%s\n", fullpath);
			}
			/*
			if (filesize(src) > 500) {
				printf("%s\n", fullpath);
			}
			*/
		}
	}
	

}
