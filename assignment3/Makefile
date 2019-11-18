CC = gcc
CFLAGS=-Wall
EXE=hash_t_test

all: $(EXE)

hash_t_test: hash_t_test.c hash_table.o
	$(CC) $(CFLAGS) $^ -o $@

hash_table.o: hash_table.c hash_table.h
	$(CC) $(CFLAGS) -c $<

.PHONY : clean
clean :
	rm -f *.o $(EXE)
