CC=gcc
CFLAGS=-g
SRC=../src
EJS=ej1 ej2 ej3 ej4
TESTS=test_ej1 test_ej2 test_ej3 test_ej4
all: $(TESTS)

test_ej1: test_ej1.c $(SRC)/generacion.o
	$(CC) $(CFLAGS) $^ -o $@
	./$@ ej1.asm
	nasm -g -o ej1.o -f elf32 ej1.asm
	gcc -m32 -o ej1 ej1.o ../lib/alfalib.o

test_ej2: test_ej2.c $(SRC)/generacion.o
	$(CC) $(CFLAGS) $^ -o $@
	./$@ ej2.asm
	nasm -g -o ej2.o -f elf32 ej2.asm
	gcc -m32 -o ej2 ej2.o ../lib/alfalib.o

test_ej3: test_ej3.c $(SRC)/generacion.o
	$(CC) $(CFLAGS) $^ -o $@
	./$@ ej3.asm
	nasm -g -o ej3.o -f elf32 ej3.asm
	gcc -m32 -o ej3 ej3.o ../lib/alfalib.o

test_ej4: test_ej4.c $(SRC)/generacion.o
	$(CC) $(CFLAGS) $^ -o $@
	./$@ ej4.asm
	nasm -g -o ej4.o -f elf32 ej4.asm
	gcc -m32 -o ej4 ej4.o ../lib/alfalib.o

opcional1: ej1_opcional.c $(SRC)/generacion.o
	$(CC) $(CFLAGS) $^ -o $@
	./$@ ej1_opcional.asm
	nasm -g -o ej1_opcional.o -f elf32 ej1_opcional.asm
	gcc -m32 -o ej1_opcional ej1_opcional.o ../lib/alfalib.o

clean:
	rm -rf *.o $(TESTS) *.asm $(EJS) ej1_opcional opcional1
