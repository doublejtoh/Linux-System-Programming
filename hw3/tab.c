#include <stdio.h>

enum { FALSE, TRUE }; // FALSE: 0, TRUE: 1

void conv(char * fname) {
	FILE * src, * dst;
	char fdest[40];
	int ch, first;
	if ((src = fopen(fname, "rt")) == NULL) {
		perror("fopen");
		return;
	}

	strcpy(fdest, fname);
	strcat(fdest, ".t"); // fdest string의 맨 뒤에 ".t"를 붙이겠다.
	if ((dst = fopen(fdest, "wt")) == NULL) {
		perror("fopen");
		return;
	}

	first = TRUE;
	while ((ch = fgetc(src)) != EOF) { // EOF가 아니면 char 1byte를 읽는다.
		if (first && ch == '\t') { //그 라인의 첫번째 글자이며, 그 글자가 '\t'이라면,
			for (int i =0; i<4; i++) { // "space 4번"
				fputc(' ', dst);
			}
		}
		else {
			fputc(ch, dst);
			if (first)
				first = FALSE;
			if (ch == '\n') // 그 라인의 끝이라면, first= TRUE 
				first = TRUE;
		}
	}

	fclose(src);
	fclose(dst);

}

main (int argc, char * argv[]) {
	while (--argc) {
		conv(argv[argc]);
	}
}
