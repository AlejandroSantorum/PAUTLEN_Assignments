#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdint.h>
#include "hash_table_com.h"

#define MAX_LOCAL_TB 1024
#define GLOBAL 0

typedef struct _symbol_tb_com symbol_tb_com;

symbol_tb_com * symb_tb_com_create();

void symb_tb_com_delete(symbol_tb_com *symb_tb);

int symb_tb_com_insert(symbol_tb_com *symb_tb, Symbol *symb);

int symb_tb_com_isKey(symbol_tb_com *symb_tb, char *key);

Symbol * symb_tb_com_search(symbol_tb_com *symb_tb, char *key, int *is_local);

Symbol ** symb_tb_com_get_list(symbol_tb_com *symb_tb, int *size, int scope);


#endif /* SYMBOL_TABLE_H */
