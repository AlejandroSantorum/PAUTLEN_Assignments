CC=gcc
CFLAGS=-g
SRC=../src
EJS=ej1 ej2 ej3 ej4 ej5 ej6 ej7
TESTS=test_ej1 test_ej2 test_ej3 test_ej4 test_ej5 test_ej6 test_ej7
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

test_ej5: test_ej5.c $(SRC)/generacion.o
	$(CC) $(CFLAGS) $^ -o $@
	./$@ ej5.asm
	nasm -g -o ej5.o -f elf32 ej5.asm
	gcc -m32 -o ej5 ej5.o ../lib/alfalib.o

test_ej6: test_ej6.c $(SRC)/generacion.o
	$(CC) $(CFLAGS) $^ -o $@
	./$@ ej6.asm
	nasm -g -o ej6.o -f elf32 ej6.asm
	gcc -m32 -o ej6 ej6.o ../lib/alfalib.o

test_ej7: test_ej7.c $(SRC)/generacion.o
	$(CC) $(CFLAGS) $^ -o $@
	./$@ ej7.asm
	nasm -g -o ej7.o -f elf32 ej7.asm
	gcc -m32 -o ej7 ej7.o ../lib/alfalib.o

opcional1: ej1_opcional.c $(SRC)/generacion.o
	$(CC) $(CFLAGS) $^ -o $@
	./$@ ej1_opcional.asm
	nasm -g -o ej1_opcional.o -f elf32 ej1_opcional.asm
	gcc -m32 -o ej1_opcional ej1_opcional.o ../lib/alfalib.o

opcional2: ej2_opcional.c $(SRC)/generacion.o
	$(CC) $(CFLAGS) $^ -o $@
	./$@ ej2_opcional.asm
	nasm -g -o ej2_opcional.o -f elf32 ej2_opcional.asm
	gcc -m32 -o ej2_opcional ej2_opcional.o ../lib/alfalib.o

opcional3: ej3_opcional.c $(SRC)/generacion.o
	$(CC) $(CFLAGS) $^ -o $@
	./$@ ej3_opcional.asm
	nasm -g -o ej3_opcional.o -f elf32 ej3_opcional.asm
	gcc -m32 -o ej3_opcional ej3_opcional.o ../lib/alfalib.o

clean:
	rm -rf *.o $(TESTS) *.asm $(EJS) ej1_opcional opcional1
