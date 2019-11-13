CC=gcc
CFLAGS=-Wall

all: tunalloc extremite

tunalloc: tunalloc.o if_tun.o
	@$(CC) -o $@ $^
	
extremite: extremite.o if_tun.o

tunalloc.o: tunalloc.c tunalloc.h if_tun.h
	$(CC) -o $@ -c $< $(CFLAGS)

if_tun.o: if_tun.c if_tun.h
	$(CC) -o $@ -c $< $(CFLAGS)

extremite.o: extremite.c extremite.h if_tun.h
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	@rm -rf *.o
	@rm -rf tunalloc
	@rm -rf extremite_exec
