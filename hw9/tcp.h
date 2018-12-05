#define MY_ID				41
#define SERV_TCP_PORT		(7000 + MY_ID)
#define SERV_HOST_ADDR		"127.0.0.1"

#define MSG_REQUEST			1
#define MSG_REPLY			2

typedef struct {
	int		type;
	char	data[128];
} MsgType;

