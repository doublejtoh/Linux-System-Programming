CC = gcc
CFLAGS = #  Compile Option. -g for gdb debugging.
LDFLAGS = #  Link Option

.SUFFIXES : .c .o
.c .o :
	$(CC) -c $(CFLAGS) $< #  $@ means target. $<: first prerequisite
ALL = mycp hole list tcp1 tcp2 mycp2 split merge conv access tab

all: $(ALL)

mycp: mycp.o
	$(CC) -o $@ $< $(LDFLAGS)

hole: hole.o
	$(CC) -o $@ $< $(LDFLAGS)

list: list.o
	$(CC) -o $@ $< $(LDFLAGS)

tcp1: tcp1.o
	$(CC) -o $@ $< $(LDFLAGS)

tcp2: tcp2.o
	$(CC) -o $@ $< $(LDFLAGS)

mycp2: mycp2.o
	$(CC) -o $@ $< $(LDFLAGS)

split: split.o
	$(CC) -o $@ $< $(LDFLAGS)

merge: merge.o
	$(CC) -o $@ $< $(LDFLAGS)

conv: conv.o
	$(CC) -o $@ $< $(LDFLAGS)

access: access.o
	$(CC) -o $@ $< $(LDFLAGS)

tab: tab.o
	$(CC) -o $@ $< $(LDGFLAGS)

clean:
	rm -rf *.o $(ALL) file.hole
	
