#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdint.h>

typedef struct _symbol_tb symbol_tb;

symbol_tb * symb_tb_create();

void symb_tb_delete(symbol_tb *symb_tb);

int symb_tb_insert(symbol_tb *symb_tb, char *key, int value);

int symb_tb_search(symbol_tb *symb_tb, char *key);


#endif /* SYMBOL_TABLE_H */
