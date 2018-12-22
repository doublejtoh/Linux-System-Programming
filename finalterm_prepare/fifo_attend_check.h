#define MAX_FIFO_NAME 32
#define SERV_FIFO "./.fifo"
#define ATTENDANCE_FILE "./attend"
typedef struct {
	char returnFifo[MAX_FIFO_NAME];
	char data[128];
	char name[128];
} MsgType;

typedef struct {
	char name[128];
	int isAttend;
} Attend;
