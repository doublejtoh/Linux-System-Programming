#include <stdio.h>
#include <pthread.h>

struct arguments {
	int start;
	int end;
	int result;
};

void add(struct arguments * arg) {
	// int sum 해서 지역변수 선언하면 안된다. pthread_exit 할 때 지역변수이므로 사라져버림. 따라서 pthread_join에서 포인터 값을 참조하면 쓰레기 값이 나오게됨. 따라서 static 변수를 이용하거나 pointer를 이용해야함.
	int i;
	for (i = arg->start; i <= arg->end; i++) {
		arg->result += i;
	}

	pthread_exit(&(arg->result));
};

main() {
	pthread_t tid1, tid2;
	void * pv[2];
	int * result[2];
	struct arguments *t1_args = malloc(sizeof(struct arguments)); //  allocates size bytes and returns a pointer to the allocated memory.
	struct arguments *t2_args = malloc(sizeof(struct arguments));

	t1_args->start = 1;
	t1_args->end = 50;
	t1_args->result = 0;

	t2_args->start = 51;
	t2_args->end = 100;
	t2_args->result = 0;

	if (pthread_create(&tid1, NULL, add, t1_args) < 0) {
		perror("pthread_create");
		exit(1);
	}

	if (pthread_create(&tid2, NULL, add, t2_args) < 0) {
		perror("pthread_create");
		exit(1);
	}
	
	printf("Threads Created: tid= %d, %d\n", tid1, tid2);

	if (pthread_join(tid1, &pv[0]) < 0) {
		perror("pthread_join");
		exit(1);
	} else {
		result[0] = pv[0];
	}

	if (pthread_join(tid2, &pv[1]) < 0) {
		perror("pthread_join");
		exit(1);
	} else {
		result[1] = pv[1];
	}
	
	printf("result: %d\n", (*(result[0]) + *(result[1])));

/*
pthread_join의 retval parameter가 void **인 이유.
pthread_exit 에서 어떤 값의 pointer를 parameter로 해서 emit하면,
pthread_join에서 retval parameter에 double pointer로 주면 (즉 pointer 변수의 주소를 주면) 그 pointer 변수의 주소로 접근하여 해당 pointer의 값에  pthread_exit에서 emit한 pointer값을 넣어주는 logic임.
https://stackoverflow.com/questions/47222124/why-the-second-argument-to-pthread-join-is-a-a-pointer-to-a-pointer
참고.

*/
	free(t1_args);
	free(t2_args);
}
