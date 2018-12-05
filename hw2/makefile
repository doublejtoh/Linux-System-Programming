CC = gcc
CFLAGS = -g
LDFLAGS = -L. -lmine
TARGET = main
OBJS = main.o

.SUFFIXES : .c .o
.c.o :
	$(CC) -c $(CFLAGS) $<

$(TARGET) : $(OBJS) libmine.a
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

libmine.a: mystrcpy.o
	ar ruv libmine.a mystrcpy.o

clean:
	rm -rf libmine.a $(TARGET) *.o
