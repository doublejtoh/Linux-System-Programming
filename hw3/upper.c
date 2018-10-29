#include <stdio.h>
#include <ctype.h>
main(int argc, char * argv[]) {
	
	FILE * src, * temp;
	char ch;

	if (argc != 2) {
		fprintf("Usage: %s source", argv[0]);
		exit(1);
	}

	if ((src = fopen(argv[1], "rt")) == NULL ) {
		perror("fopen");
		exit(1);
	}
	
	if ((temp = fopen("temp", "wt")) == NULL) {
		perror("fopen");
		exit(1);
	}

	while ((ch = fgetc(src)) != EOF) {
		putc(toupper(ch), temp);
	}

	fclose(src);
	fclose(temp);

	
	remove(argv[1]);
	rename("temp", argv[1]);

}
