#include <stdio.h>


void del (char str[]) {

	int len = strlen(str);
	int i;

	for (i = 0; i < len-1; i++) {
		str[i] = str[i+1];
	}
	
	str[i] = '\0';
}

main() {
	char str[20] = "Hello";

	del(str);
	puts(str);
	printf("%d:\n",strlen(str));

}
