#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdint.h>

typedef struct _hash_tb hash_tb;


hash_tb * hash_tb_create(int ht_sz);

void hash_tb_delete(hash_tb *ht);

int hash_tb_insert(hash_tb *ht, char *key, int value);

int hash_tb_print(hash_tb *ht);


#endif /* HASH_TABLE_H */
