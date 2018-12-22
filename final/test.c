#include <stdio.h>

main() {
	char a;
	while(1) {
		fgets(a, 1, stdin);
		printf("%s",a);
	}
}
