#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdint.h>

typedef struct _ht_item{
    char *key;
    int value;
}ht_item;

typedef struct _ht_arr{
    ht_item** item_arr;
    size_t chain_base_sz;
    size_t curr_sz;
    size_t insert_idx;
    size_t dyn_resz;
} ht_arr;

struct _hash_tb{
    ht_arr **ht_arr;
    size_t ht_sz;
};

typedef struct _hash_tb hash_tb;


hash_tb * hash_tb_create(size_t ht_sz, size_t chain_sz, size_t dyn_resz);

void hash_tb_delete(hash_tb *ht);

int hash_tb_print(hash_tb *ht);

int hash_tb_insert(hash_tb *ht, char *key, int value);

int hash_tb_isKey(hash_tb *ht, char *key);

int hash_tb_get(hash_tb *ht, char *key, int *value);


#endif /* HASH_TABLE_H */
