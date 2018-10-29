#include <stdio.h>

#define MAX_CMD 256

void DoCmd(char * cmd) {
	printf("Doing %s", cmd);
	sleep(1);
	printf("Done\n");
}

main () {
	char cmd[MAX_CMD];

	while (1) {
		printf("CMD> ");
		fgets(cmd, MAX_CMD, stdin); // stdin에 MAX_CMD byte만큼 입력을 받아서 cmd에 저장.
		if (cmd[0] == 'q')
			break;
		DoCmd(cmd);
	}

}
