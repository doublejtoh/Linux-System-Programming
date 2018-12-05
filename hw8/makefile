CC = gcc
CFLAGS =
LDFLAGS = -lpthread

.SUFFIXES : .c .o
.c.o :
	$(CC) -c $(CFLAGS) $<

ALL = producer consumer producer_s consumer_s prodcons dining dining2 prodcons_m prodcons_s sipc_process_semaphore_1.c sipc_process_semaphore_2.c sipc_thread_semaphore.c

all: $(ALL)

prodcons_t: prodcons_t.o
	$(CC) -o $@ $< $(LDFLAGS)

producer: producer.o 
	$(CC) -o $@ $< $(LDFLAGS)

consumer: consumer.o 
	$(CC) -o $@ $< $(LDFLAGS)

producer_s: producer_s.o semlib.o
	$(CC) -o $@ $? $(LDFLAGS)

consumer_s: consumer_s.o semlib.o
	$(CC) -o $@ $? $(LDFLAGS)

prodcons: prodcons.o
	$(CC) -o $@ $< $(LDFLAGS)

dining: dining.o
	$(CC) -o $@ $< $(LDFLAGS)

dining2: dining2.o
	$(CC) -o $@ $< $(LDFLAGS)

prodcons_m: prodcons_m.o
	$(CC) -o $@ $< $(LDFLAGS)

prodcons_s: prodcons_s.o semlib2.o
	$(CC) -o $@ $? $(LDFLAGS)

sipc_process_semaphore_1: sipc_process_semaphore_1.o semlib.o
	$(CC) -o $@ $? $(LDFLAGS)

sipc_process_semaphore_2: sipc_process_semaphore_2.o semlib.o
	$(CC) -o $@ $? $(LDFLAGS)

sipc_thread_semaphore: sipc_thread_semaphore.o semlib2.o
	$(CC) -o $@ $? $(LDFLAGS) -g -ggdb

clean :
	rm -rf *.o $(ALL)
