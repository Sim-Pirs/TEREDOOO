EXEC=tunalloc
SRC= $( *.c)
OBJ= $(SRC:.c=.o)

all: $(EXEC)

hello: $(OBJ)
	@$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf $(EXEC)
