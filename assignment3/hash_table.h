#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdint.h>

typedef struct _hash_tb hash_tb;


hash_tb * hash_tb_create(int ht_sz);

void hash_tb_delete(hash_tb *ht);


#endif /* HASH_TABLE_H */
