CC = gcc
CFLAGS=-Wall
EXE=hash_t_test symbol_t_test prueba_tabla

all: $(EXE)

prueba_tabla: prueba_tabla.c hash_table.o symbol_table.o
	$(CC) $(CFLAGS) $^ -o $@

symbol_t_test: symbol_t_test.c hash_table.o symbol_table.o
	$(CC) $(CFLAGS) $^ -o $@

hash_t_test: hash_t_test.c hash_table.o
	$(CC) $(CFLAGS) $^ -o $@

hash_table.o: hash_table.c hash_table.h
	$(CC) $(CFLAGS) -c $<

symbol_table.o: symbol_table.c symbol_table.h
	$(CC) $(CFLAGS) -c $<

.PHONY : clean
clean :
	rm -f *.o $(EXE)
