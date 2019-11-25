#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdint.h>
#include "hash_table.h"

#define MAX_LOCAL_TB 1024

struct _symbol_tb{
    hash_tb *global;
    hash_tb *local[MAX_LOCAL_TB];
    int current_local_tb;
};

typedef struct _symbol_tb symbol_tb;

symbol_tb * symb_tb_create();

void symb_tb_delete(symbol_tb *symb_tb);

int symb_tb_insert(symbol_tb *symb_tb, char *key, int value);

int symb_tb_isKey(symbol_tb *symb_tb, char *key);

int symb_tb_search(symbol_tb *symb_tb, char *key, int *value);


#endif /* SYMBOL_TABLE_H */
