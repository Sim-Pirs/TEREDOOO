EXEC=tunalloc
CC=gcc
CFLAGS=-W -Wall
LDFLAGS=
OBJ=$(SRC:.c=.o)

all: $(EXEC)

tunalloc: tunalloc.o extremite.o
	@$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf $(EXEC)
