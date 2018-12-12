CC = gcc
CFLAGS =
LDFLAGS = 

.SUFFIXES : .c .o
.c.o :
	$(CC) -c $(CFLAGS) $<

ALL = pipe sync mymore fifos fifoc msgq1 msgq2 shm sipc1 sipc2 mycp3 mipc

all: $(ALL)

pipe: pipe.o
	$(CC) -o $@ $< $(LDFLAGS)

pipe_hw: pipe_hw.o
	$(CC) -o $@ $< $(LDFLAGS)

sync: sync.o synclib.o
	$(CC) -o $@ sync.o synclib.o $(LDFLAGS)

mymore: mymore.o
	$(CC) -o $@ $< $(LDFLAGS)

fifos: fifos.o
	$(CC) -o $@ $< $(LDFLAGS)

fifoc: fifoc.o
	$(CC) -o $@ $< $(LDFLAGS)

msgq1: msgq1.o 
	$(CC) -o $@ $< $(LDFLAGS)

msgq2: msgq2.o 
	$(CC) -o $@ $< $(LDFLAGS)

shm: shm.o 
	$(CC) -o $@ $< $(LDFLAGS)

sipc1: sipc1.o
	$(CC) -o $@ $< $(LDFLAGS)

sipc2: sipc2.o
	$(CC) -o $@ $< $(LDFLAGS)

mycp3: mycp3.o
	$(CC) -o $@ $< $(LDFLAGS)

mipc: mipc.o synclib.o
	$(CC) -g -o $@ mipc.o synclib.o $(LDFLAGS)

mipc_hw: mipc_hw.o synclib.o
	$(CC) -o $@ mipc_hw.o synclib.o $(LDFLAGS)

mylsgrep: mylsgrep.o
	$(CC) -o $@ $< $(LDFLAGS)

clean :
	rm -rf *.o $(ALL)
