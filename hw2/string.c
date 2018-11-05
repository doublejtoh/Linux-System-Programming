#include <stdio.h>

int strlen_p(char *str) { // string의 길이를 return해주는 함수
	int len = 0;
	while (*str++) { // string *str가 null이 아니라면, 계속 while을 돈다.
		len++;
	}
	return len;
}

int strlen_a(char str[]) {
	int i;
	for (i = 0; str[i] != '\0'; i++);
	return i;
}

void strcpy_p(char *dst, char *src) { // src char 배열의 각 원소의 값을 dst char 배열의 각 원소의 값으로 넣어준다.
	while (*src) {
		*dst++ = *src++;
	}
	*dst = *src;
}

void strcpy_a(char dst[], char src[]) { // 위의 함수와 같은 동작을 한다. 대신 array를 이용한다.
	int i;
	for (i = 0; src[i] != '\0'; i++) {
		dst[i] = src[i];
	}
	dst[i] = src[i];
}

void strcat_p(char *dst, char *src) { // dst char 배열에 src char 배열을 덧붙인다.
	while (*dst++); // dst배열이 null을 만날때 까지 pointer를 +1 해준다.
	dst--; //dst pointer의 값을 -1 해준다.
	while (*src) { // src배열의 값이 null을 만날 떄 까지 while을 돈다.
		*dst++ = *src++; //  dsc, src 포인터 값을 +1 해준다.
	}
	*dst = *src; // null 값을 dst 배열의 끝에 넣어준다.
}

void strcat_a(char dst[], char src[]) { // dst char 배열에 src char 배열을 덧붙인다.
	int i,j;
	for (i = 0; dst[i] != '\0'; i++); // dst 배열이 null을 만날 떄 까지 index i 를 +1 해준다.
	for (j = 0; src[j] != '\0'; j++) // src 배열이 null을 만날 떄 까지 index j를 +1 해준다.
		dst[i+j] = src[j]; 
	dst[i+j] = src[j]; // null 값을 dst 배열의 끝에 넣어준다.
}

////////// 추가한 함수////////////

int strcmp_p(char *dst, char *src) { 
	while(*dst && *src) { // dst배열의 원소와 src배열의 원소 둘다가 null값이 아니면 계속 while문을 돈다.
		if (*dst == *src) { // 만약 dst배열의 원소와 src배열의 원소가 같다면
			dst++; // 포인터를 +1
			src++; // 포인터를 +1
		} else { // 아니라면 
			return *dst - *src; //( ascii code 값): dst배열 원소 - src 배열 원소 즉, dst의 ascii code 값이 더크면 양수, 작다면 음수를 return.
		}
	}
	return 0; // 끝까지 문자열을 탐색했는데 다 같다면 0을 리턴
}

int strcmp_a(char dst[], char src[]) {
	int i = 0;
	while(dst[i] && src[i]) { //dst 배열의 원소와 src배열의 원소 둘다가 null값이 아니면 계속 while문을 돈다.
		if (dst[i] == src[i]) { //만약 dst배열의 원소와 src배열의 원소가 같다면
			i++; // index를 +1
		} else { //아니라면
			return dst[i] - src[i]; // ascii code 값 dst 배열 원소 - src 배열 원소 값
		}
	}
	return 0; // 끝까지 문자열을 탐색했는데 다 같으면 0을 리턴
}

main() {
	int len1, len2;
	int compare;
	char str1[20], str2[20];

	len1 = strlen_p("Hello");
	len2 = strlen_a("Hello");
	printf("strlen: p=%d, a=%d\n", len1, len2);

	strcpy_p(str1, "hello");
	strcpy_a(str2, "hello");
	printf("strcpy: p=%s, a=%s\n", str1, str2);
	
	strcat_p(str1, ", World!");
	strcat_a(str2, ", World!");
	printf("strcat: p=%s, a=%s\n", str1, str2);
	
	compare = strcmp_p(str1, str2);
	if (compare > 0) {
		printf("%s가 %s보다 더 큽니다.\n", str1, str2);
	} else if (compare < 0) {
		printf("%s가 %s보다 더 작습니다.\n", str1, str2);
	} else {
		printf("%s와 %s는 같습니다.\n", str1, str2);
	}
}	
