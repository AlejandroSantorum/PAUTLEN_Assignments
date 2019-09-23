CC=gcc
CFLAGS=-g
SRC=../src

test_generacion: test_generacion.c $(SRC)/generacion.o
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf *.o test_generacion
