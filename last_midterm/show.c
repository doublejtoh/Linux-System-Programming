#include <stdio.h>

main () {
	for (int i = 9; i > 0 ; i--) {
		for (int j = 1 ; j <= i; j++) {
			printf("%d ", j);
		}
		printf("\n");
	} 

}
