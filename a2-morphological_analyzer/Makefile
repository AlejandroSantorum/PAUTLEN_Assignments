CC=gcc
CFLAGS=-Wall

all: pruebaMorfo

pruebaMorfo: pruebaMorfo.c lex.yy.o
	$(CC) $(CFLAGS) $^ -o $@

lex.yy.o: lex.yy.c
	$(CC) $(CFLAGS) -c $^

lex.yy.c: alfa.l
	flex $^

.PHONY: clean
clean:
	rm -rf *.o pruebaMorfo lex.yy.*
