CC = gcc
CFLAGS = 
LDFLAGS = -lpthread

.SUFFIXES : .c .o
.c.o:
	$(CC) -c $(CFLAGS) $<

ALL = sum mystrcasestr mygrep cmd alarm

all: $(ALL)

sum: sum.o
	$(CC) -o $@ $< $(LDFLAGS)

mystrcasestr: mystrcasestr.o
	$(CC) -o $@ $< $(LDFLAGS)

mygrep: mygrep.o
	$(CC) -o $@ $< $(LDFLAGS)

cmd: cmd.o
	$(CC) -o $@ $< $(LDFLAGS)

alarm: alarm.o
	$(CC) -o $@ $< $(LDFLAGS)
	

clean:
	rm -rf *.o $(ALL)
