#include <stdio.h>
#include <sys/wait.h>
#include <errno.h>

int mysystem(char * cmd) {

	pid_t  pid;
	int ret;
	int status;
	if (cmd == NULL) { // command가 null이라면
		return -1;
	}
	char * args [] = { "sh", "-c", cmd, NULL};
	if ((pid = fork()) < 0) {
		perror("fork");
		
		return -1;
	}

	else if (pid == 0) {
		
		if (execv("/bin/sh", args) < 0) {
			perror("execv");
			exit(1);
		}
		
	}



	else {
		while(1) {
			if((ret = waitpid(pid, &status, 0)) == -1) {
				if (errno != EINTR)
					return -1;	
			} else {
				if WIFEXITED(status) // process exited successfully.
					return WEXITSTATUS(status);
				else
					return status;
			}
		}
	}	
	
}


main(int argc, char * argv[]) {
	
	if (argc < 2) {
		printf("Usage: mysystem cmd");
		exit(1);
	}

	int status;
	status = mysystem(argv[1]);
	printf("%s exited with status : %d\n", argv[1], status);
	 

}
