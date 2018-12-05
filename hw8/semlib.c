#include <stdio.h>
#include "semlib.h"

int semInit(key_t key) {
	int semid;
	if ((semid = semget(key, 1, 0600 | IPC_CREAT)) < 0) { // semget: semaphore를 하나 만들거나 get. 두번쨰 파라미터에는 만들 semaphore의 개수임. 세번째 파라미터는 "owner"만 read,write할 수있도록 하며 semaphore가 존재하지 않았다면 create하라는 뜻.
		perror("shmget");
		return -1;
	} // semget 의 initial value는 0이다.

	return semid; // semget의 return value는 semaphore의 id이다.
}

int semInitValue(int semid, int value) {
	union semun { // union은 공용체는 멤버중에서 가장 큰 자료형의 공간으로 memory에 잡히게 됨. 따라서 어느 한 멤버에 값을 저장하면 나머지 멤버의 값은 사용할 수 없음. 따라서 공용체에 값을 저장할 때 어떤 멤버를 사용할 것인지 미리 정해놓고, 꺼낼때도 정해놓은 멤버에서 값을 꺼내는 식으로 사용해야함.
		int val;
	} semun;

	semun.val = value;
	if (semctl(semid, 0, SETVAL, semun) < 0) { // semctl를 통해서 특정 semaphore의 값을 변경시킴.
		perror("semctl");
		return -1;
	}

	return semid;

}

int semWait(int semid) {
	struct sembuf semcmd;
	
	semcmd.sem_num = 0; // 생성한 semaphore중에서 operation을 적용할 semaphore의 idx.
	semcmd.sem_op = -1; // Wait operation.
	semcmd.sem_flg = SEM_UNDO; // 만약 semaphore를 가지고 있다가 반납을 안했는데 프로세스가 비정상종료되는 상황을 방지하여 자동적으로 반납하도록 하는 것이 SEM_UNDO.
	if (semop(semid, &semcmd, 1) < 0) { // 마지막 파라미터는 적용할 operation의 개수.
		perror("semop");
		return -1;
	} 

	return 0;
}

int semTryWait(int semid) {
	struct sembuf semcmd;
	
	semcmd.sem_num = 0; // semaphore idx.
	semcmd.sem_op = -1;
	semcmd.sem_flg = IPC_NOWAIT | SEM_UNDO; // IPC_NOWAIT: semaphore가 0일때 wait하지않고 fail처리하도록 한다.
	if (semop(semid, &semcmd, 1) < 0) {
		perror("semop");
		return -1;
	}

	return 0;
}

int semPost(int semid) {
	struct sembuf semcmd;

	semcmd.sem_num = 0;
	semcmd.sem_op = 1;
	if (semop(semid, &semcmd, 1) < 0) {
		perror("semop");
		return -1;
	}

	return 0;
}

int semGetValue(int semid) {
	union semun {
		int val;
	} dummy;

	return semctl(semid, 0, GETVAL, dummy); // semid, semIDX, GETVAL, dummy. 현재 semaphore의 값을 return.
}

int semDestroy(int semid) {
	union semun{
		int val;
	} dummy;

	if (semctl(semid, 0, IPC_RMID, dummy) < 0) {
		perror("semctl");
		return -1;
	}

	close(semid);
	return 0;
}
