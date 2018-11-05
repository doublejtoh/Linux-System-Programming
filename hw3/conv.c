#include <stdio.h>
#include <string.h>
#include "record.h"

main (int argc, char * argv[]) {
	FILE * src, *dst;
	Record rec;

	if (argc != 3) {
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	if ((src = fopen(argv[1], "rt")) == NULL ){
		perror("fopen");
		exit(1);
	}

	if ((dst = fopen(argv[2], "wb")) == NULL) {
		perror("fopen");
		exit(1);
	}

	while (fgets(rec.stud, SMAX, src)) { // SMAX - 1만큼 실제로 문자를 읽고, 나머지 1개는 '\n'을 읽는다.
		*strchr(rec.stud, '\n') = '\0'; // The  strchr() function returns a pointer to the first occurrence of target char in the string s.
		fgets(rec.num, NMAX, src);
		*strchr(rec.num, '\n') = '\0'; // binary file에서는 '\n'이 무의미 하기 때문에 '\0'로 replace.
		fgets(rec.dept, DMAX, src);
		*strchr(rec.dept, '\n') = '\0';
		fwrite(&rec, sizeof(rec), 1, dst); // rec 객체의 주소를 넘겨주고, rec객체의 size를 알려줘서 dst binary file에 적도록한다.
	}

	fclose(src);
	fclose(dst);
}
