CC = gcc
CFLAGS =
LDFLAGS = 

.SUFFIXES : .c .o
.c.o :
	$(CC) -c $(CFLAGS) $<

ALL = order tcps tcps_p tcpc udps udpc ucos ucoc ucls uclc tcpc_dns myusleep select sgs sgc

all: $(ALL)

order: order.o 
	$(CC) -o $@ $< $(LDFLAGS)

tcps: tcps.o 
	$(CC) -o $@ $< $(LDFLAGS)

tcps_p: tcps_p.o
	$(CC) -o $@ $< $(LDFLAGS)

tcpc: tcpc.o 
	$(CC) -o $@ $< $(LDFLAGS)

udps: udps.o 
	$(CC) -o $@ $< $(LDFLAGS)

udpc: udpc.o 
	$(CC) -o $@ $< $(LDFLAGS)

ucos: ucos.o 
	$(CC) -o $@ $< $(LDFLAGS)

ucoc: ucoc.o 
	$(CC) -o $@ $< $(LDFLAGS)

ucls: ucls.o 
	$(CC) -o $@ $< $(LDFLAGS)

uclc: uclc.o 
	$(CC) -o $@ $< $(LDFLAGS)

tcpc_dns: tcpc_dns.o 
	$(CC) -o $@ $< $(LDFLAGS)

myusleep: myusleep.o 
	$(CC) -o $@ $< $(LDFLAGS)

select: select.o 
	$(CC) -o $@ $< $(LDFLAGS)

sgs: sgs.o 
	$(CC) -o $@ $< $(LDFLAGS)

sgc: sgc.o 
	$(CC) -o $@ $< $(LDFLAGS)

clean :
	rm -rf *.o $(ALL)
