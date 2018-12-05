#define MY_ID		41
#define SHM_KEY		(0x9000 + MY_ID)
#define SHM_MODE	(SHM_R | SHM_W | IPC_CREAT)

#define EMPTY_SEM_KEY	(0x5000 + MY_ID)
#define FULL_SEM_KEY	(0x6000 + MY_ID)
#define MUTEX_SEM_KEY	(0x7000 + MY_ID)

#define NLOOPS			20
#define MAX_BUF			2 // circular Queue의 max length를 10으로 하지 않은 이유는 10으로 하면 대개 꽉 채워지거나, 아예 비어지는 경우가 생기지 않음. 평균적으로 4~6개 정도로 producer와 consumer가 유지하게됨.

typedef struct {
	int data;
} ItemType;

typedef struct {
	ItemType	buf[MAX_BUF]; // current Circular Queue
	int			in; // current idx of in 
	int			out; // current idx of out
	int			counter; // current length of circular Queue
} BoundedBufferType;

#define SHM_SIZE		sizeof(BoundedBufferType)

