#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shm.h"

main() {
	int shmid;
	char *ptr, *pData;
	int *pInt;

	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0) { // get or create Shared memory.
		perror("shmget");
		exit(1);
	}

	if ((ptr = shmat(shmid, 0, 0)) == (void * ) - 1) {
		perror("shmat");
		exit(1);
	}

	pInt = (int *) ptr;
	pData = ptr + sizeof(int);
	sprintf(pData, "This is a request from %d.", getpid());
	*pInt = 1; // *pInt의 값을 1로 바꿔서 sipc1이 while 문 벗어날 수 있도록 함.
	printf("Sent a request.......");

	while ((*pInt) == 1) // busy waiting.
		;

	printf("Received reply: %s\n", pData);
	printf("ptr: %x\n", ptr);
}


/*

* sipc1와 sipc2중 sipc1이 먼저 수행되는 상황:

만약 sipc1이 먼저 실행되면, sipc1은 while문에서 busy waiting하고 있게 됨.
timer interrupt걸려서 운영체제가 sipc2를 scheduling하면 *pInt를 1로 바꾸고, pData에 값을 채워서 request 보낸 후 ,sipc2는 무한루프에 들어가게 됨. 똑같이 sipc1가 수행되면 while문벗어나서 pInt를 0으로 채우고, pData에 값을 채워서 reply한 후, sleep(1)하면 다시 sipc2가 수행 되서 while문 벗어나서 printf("Received reply:"가 수행되고 sipc2 끝나고, sipc1도 끝나게 된다.

* sipc2가 먼저 수행되는 상황:

sipc2는 *pInt를 1로 채우고, pData에 값을 채워서 request보낸후 while문에서 busy waiting하다가, timer interrupt 걸려서 운영체제가 sipc1을 수행하면 while문에서 바로 빠져나와서 pData를 채우고 pInt를 0으로 바꾼후 reply를 보내고 1초동안 sleep하면 sipc2이 수행되서 reply받았다고 출력하고 sipc2는 종료, 그다음에 sipc1도 종료됨.


* sipc1과 sipc2중 둘중 하나만 shmctl하면 된다. 둘다 하면 에러.

*/
