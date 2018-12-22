
CC = gcc
CFLAGS =
LDFLAGS =  -lpthread

.SUFFIXES : .c .o
.c.o :
	$(CC) -c $(CFLAGS) $<

ALL = mylst rwp server client 

all: $(ALL)

mylst: mylst.o
	$(CC) -o $@ $< $(LDFLAGS)

rwp: rwp.o
	$(CC) -o $@ $< $(LDFLAGS)

server: server.o
	$(CC) -o $@ $< $(LDFLAGS) 

client: client.o
	$(CC) -o $@ $< $(LDFLAGS)

clean :
	rm -rf *.o $(ALL)
