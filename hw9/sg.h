#define	MY_ID			0

#define	SERV_TCP_PORT	(9000 + MY_ID)
#define	SERV_HOST_ADDR	"127.0.0.1"

#define	MSG_REQUEST		1
#define	MSG_REPLY		2


typedef struct  {
	int		type;
	char	data[128];
}
	MsgType;

typedef struct  {
	char	info[8];
}
	HeaderType;
