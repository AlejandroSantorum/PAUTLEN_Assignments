CC=gcc
CFLAGS=-Wall -g
MINI = mini mini2 mini3 mini4 mini5 mini6 mini7 mini8 mini9 factorial condicionales vectores fibonacci funciones_vectores funciones test
ERR = err1 err2 err3 err4 err5 err6 err7 err8 err9 err10 err11 err12 err13 err14 err15 err16
EXE = alfa $(MINI)

all: $(EXE)

alfa: alfa.c lex.yy.o y.tab.o generacion.o symbol_table_com.o hash_table_com.o
	$(CC) $(CFLAGS) $^ -o $@

generacion.o: generacion.c generacion.h
	$(CC) $(CFLAGS) -c $<

symbol_table_com.o: symbol_table_com.c symbol_table_com.h
	$(CC) $(CFLAGS) -c $<

hash_table_com.o: hash_table_com.c hash_table_com.h
	$(CC) $(CFLAGS) -c $<

lex.yy.o: lex.yy.c
	$(CC) $(CFLAGS) -c $^

y.tab.o: y.tab.c
	$(CC) $(CFLAGS) -c $<

lex.yy.c: alfa.l y.tab.h
	flex $<

y.tab.c: alfa.y
	bison -d -y -v $^

y.tab.h: alfa.y
	bison -d -y -v $^

${MINI}: %: alfa examples/%.alfa
	./alfa examples/$@.alfa examples/$@.asm
	nasm -g -o examples/$@.o -f elf32 examples/$@.asm
	gcc -m32 -o examples/$@ examples/$@.o ../alfalib.o

${ERR}: %: alfa errors/%.alfa
	./alfa errors/$@.alfa errors/$@.asm
	nasm -g -o errors/$@.o -f elf32 errors/$@.asm
	gcc -m32 -o errors/$@ errors/$@.o ../alfalib.o

alfa.c:
	@echo "Empty" > /dev/null

.PHONY: clean
clean:
	rm -rf *.o alfa lex.yy.* y.tab.* y.output *.asm
	cd errors && $(MAKE) clean
	cd examples && $(MAKE) clean
