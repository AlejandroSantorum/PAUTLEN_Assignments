CC=gcc
CFLAGS=-g

all: generacion.o

generacion.o: generacion.c
	$(CC) $(CFLAGS) -c $?

.PHONY: clean
clean:
	rm -rf *.o
