#ifndef ALFA_H
#define ALFA_H

#include "hash_table_com.h"
#include "generacion.h"
#include "symbol_table_com.h"

#define MAX_LONG_ID 100
#define MAX_VECTOR_SIZE 64

typedef struct _attr_type {
    char lexeme[MAX_LONG_ID+1];
    char vector_index[MAX_LONG_ID+1];
    int type;
    int int_value;
    int is_address;
    int vector_index_is_address;
    int label;
    int vec_size;
} attr_type;

#endif /* ALFA_H */
