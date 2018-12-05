#include <stdio.h>
#include <sys/types.h>

main (int argc, char * argv[]) {

	pid_t pid;

	if (argc <  2) {
		fprintf(stderr, "Usage: %s target\n", argv[0]);
		exit(1);
	}

	
	char * args[] = { "rm", "-rf" };
	printf("sizeof(args): %d, sizeof(args[0]) : %d\n ", sizeof(args), sizeof(args[0]));
	int size_of_args = sizeof(args)/sizeof(args[0]);
	char ** concated;
	concated = (char(*)[])malloc(sizeof(char*)*(argc+size_of_args-1));
	
	int i;
	for( i =0 ; i <size_of_args; i++) {
		concated[i] = args[i];
	}
	int j;
	for (j=0; j < argc; j++) {
		
		concated[j+i] = argv[j+1];
	}
	
	for (int z =0; z < argc+size_of_args; z++) {
		printf("concated: %s\n", concated[z]);
	}
	
	if ((pid = fork()) <0 ) {
		perror("fork");
		exit(1);
	}

	else if (pid == 0) {
		execv("/bin/rm", concated);
			
	}

	else {
		waitpid(pid);
		return;
		
	}
}
