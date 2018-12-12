CC = gcc
CFLAGS = 
LDFLAGS =

.SUFFIXES : .c .o
.c.o :
	$(CC) -c $(CFLAGS) $<

ALL = fifo_attend_check_server fifo_attend_check_client

all: $(ALL)

fifo_attend_check_server: fifo_attend_check_server.o
	$(CC) -o $@ $< $(LDFLAGS)

fifo_attend_check_client: fifo_attend_check_client.o
	$(CC) -o $@ $< $(LDFLAGS)

clean:
	rm -rf *.o $(ALL)

