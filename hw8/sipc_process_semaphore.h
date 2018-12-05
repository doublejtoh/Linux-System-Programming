#define MY_ID		41
#define SIPC1_SEM_KEY (0x5000 + MY_ID)
#define SIPC2_SEM_KEY (0x6000 + MY_ID)
#define MUTEX_SEM_KEY (0x7000 + MY_ID)
#define SHM_KEY		(0x9000 + MY_ID)
#define SHM_SIZE	1024
#define SHM_MODE	(SHM_R | SHM_W | IPC_CREAT)

