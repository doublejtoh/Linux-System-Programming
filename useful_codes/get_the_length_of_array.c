#include <stdio.h>

main () {

int a[17];

size_t n = sizeof(a) / sizeof(a[0]);
printf("%d\n", n);
}
