CC=gcc
CFLAGS=-Wall

all: pruebaSintactico

pruebaSintactico: pruebaSintactico.c lex.yy.o y.tab.o
	$(CC) $(CFLAGS) $^ -o $@

lex.yy.o: lex.yy.c
	$(CC) $(CFLAGS) -c $^

y.tab.o: y.tab.c
	$(CC) $(CFLAGS) -c $<

lex.yy.c: alfa.l y.tab.h
	flex $<

y.tab.c: alfa.y
	bison -d -y $^

y.tab.h: alfa.y
	bison -d -y -v $^

.PHONY: clean
clean:
	rm -rf *.o pruebaSintactico lex.yy.* y.tab.* y.output
