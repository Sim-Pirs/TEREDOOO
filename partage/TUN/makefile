CC=gcc
CFLAGS=-Wall

all: tunalloc extremite_exec

tunalloc: tunalloc.o if_tun.o
	@$(CC) -o $@ $^

tunalloc.o: tunalloc.c tunalloc.h if_tun.h
	$(CC) -o $@ -c $< $(CFLAGS)

if_tun.o: if_tun.c if_tun.h
	$(CC) -o $@ -c $< $(CFLAGS)

extremite_exec: extremite_exec.o extremite.o if_tun.o
	@$(CC) -o $@ $^

extremite.o: extremite.c extremite.h if_tun.h
	$(CC) -o $@ -c $< $(CFLAGS)

extremite_exec.o: extremite_exec.c extremite_exec.h if_tun.h extremite.h
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	@rm -rf *.o
	@rm -rf tunalloc
	@rm -rf extremite_exec
