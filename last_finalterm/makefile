CC = gcc
CFLAGS =
LDFLAGS = -lpthread

.SUFFIXES : .c .o
.c.o:
	$(CC) -c $(CFLAGS) $<

ALL = del fifo1 fifo2 thread server client

all: $(ALL)

del: del.o
	$(CC) -o $@ $< $(LDFLAGS)

fifo1: fifo1.o
	$(CC) -o $@ $< $(LDFLAGS)

fifo2: fifo2.o
	$(CC) -o $@ $< $(LDFLAGS)

thread: thread.o
	$(CC) -o $@ $< $(LDFLAGS)

server: server.o
	$(CC) -o $@ $< $(LDFLAGS)

client: client.o
	$(CC) -o $@ $< $(LDFLAGS)

test: test.o
	$(CC) -o $@ $< $(LDFLAGS)

clean:
	rm -rf *.o $(ALL)

