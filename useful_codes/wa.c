#include <stdio.h>
#include <dirent.h>
#include <unistd.h>

main() {
	DIR * dp;
	struct dirent * dep;

	if ((dp = opendir("/home")) == NULL) {
		perror("opendir");
		exit(1);
	}

	while ((dep = readdir(dp))) {
		printf("%s \n", dep->d_name);
	}
}
