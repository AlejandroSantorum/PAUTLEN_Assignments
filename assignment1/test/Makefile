CC=gcc
CFLAGS=-g
SRC=../src
EJS=ej1
TESTS=test_ej1
all: $(TESTS)

test_ej1: test_ej1.c $(SRC)/generacion.o
	$(CC) $(CFLAGS) $^ -o $@
	./$@ ej1.asm
	nasm -g -o ej1.o -f elf32 ej1.asm
	gcc -m32 -o ej1 ej1.o ../lib/alfalib.o

clean:
	rm -rf *.o $(TESTS) *.asm $(EJS)
