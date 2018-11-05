#include <stdio.h>
#include <string.h>

char * mystrcasestr(char str1[], char str2[]) {
	int i, j;
	int sizeofstr2 = strlen(str2); // str2 (찾아야하는 str)의 길이
	char substring[sizeofstr2]; // str1에서 substring하는데, str2의 길이와 동일하다.
	for (i = 0; str1[i] != '\0'; i++) {
		for (j = 0; j < sizeofstr2; j++) { // For문을 통해서 str1를 substring하면서 substring에 저장하는데, 이 때 시작 위치와 str2의 길이를 가지고 substring한다.
			substring[j] = str1[j+i];
		}
		if (strcasecmp(substring, str2) == 0) { // 추출된 substring과 str2를 대소문자를 ignore하면서 string을 비교한다. 만약 같다면, str1[i]의 주소를 리턴해준다. 즉, str1의 시작주소를 리턴해준다.
			return &str1[i];
		}
	}

	return NULL; // 만약 같지 않다면, NULL값을 리턴한다.
}

main () {
	
	char str[] = "Hello hello HELLO";

	char * p;

	p = str;

	while(p) {
		p = mystrcasestr(p, "hello");
		if (p != NULL) {
			puts(p);
			p++;
		}	
	}

}
