#include <stdio.h>

main(int argc, char * argv[]){
	FILE * src, * dst;
	int ch;
	if (argc != 3){
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	if ((src = fopen(argv[1], "rt")) == NULL) { // read text모드로 오픈하겠다. 만약 NULL값이 return된다면 if문으로 빠짐.
		perror("fopen");
		exit(1);
	}

	if ((dst = fopen(argv[2], "wt")) == NULL) {
		perror("fopen");
		exit(1);
	}

	while ((ch = fgetc(src)) != EOF) {
		fputc(ch, dst); // ch가 int인 이유: fget가 EOF를 반환하도록 되어있는데, 이에맞춰서 fputc도 int를받도록 설계를 한듯. 
	}
	
	fclose(src);
	fclose(dst);

}
