CC=gcc
CFLAGS=-Wall
MINI = mini mini2 mini3 mini4 mini5 mini6 factorial condicionales vectores fibonacci
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

${MINI}: %: alfa %.alfa
	./alfa $@.alfa $@.asm
	nasm -g -o $@.o -f elf32 $@.asm
	gcc -m32 -o $@ $@.o ../alfalib.o

alfa.c:
	@echo "Empty" > /dev/null

.PHONY: clean
clean:
	rm -rf *.o alfa lex.yy.* y.tab.* y.output $(EXE)
