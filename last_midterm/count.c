#include <stdio.h>

typedef enum {FALSE = 0, TRUE} boolean;

main (int argc, char * argv[]) {
	FILE * src;
	char ch;
	int count = 0 ;
	boolean isCharFound = FALSE;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s source \n", argv[0]);
		exit(1);
	}

	if ((src = fopen(argv[1], "rt")) == NULL) {
		perror("fopen");
		exit(1);
	}

	while ((ch = fgetc(src)) != EOF) {
		if (isCharFound) {
			if (ch == ' ' || ch == '\n') {
				count++;
			}
		}
		if(ch != ' ' && ch != '\n') {
			isCharFound = TRUE;
		}
	}
	
	printf("%d\n", count);

}
