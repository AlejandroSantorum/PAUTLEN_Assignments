#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

#define HASHCODE_LEN 3

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



int _hash_code(char *key, size_t size){
    int l, i;
    int acum=0;
    if(!key){
        perror("NULL key at hash_code function");
        return -1;
    }

    l = strlen(key);

    if(l<HASHCODE_LEN){
        for(i=0; i<l; i++){
            acum += (int) key[i];
        }
    }else{
        for(i=0; i<HASHCODE_LEN; i++){
            acum += (int) key[i];
        }
    }
    return acum%size;
}


ht_item * _ht_item_create(){
    ht_item *item=NULL;

    item = (ht_item *) calloc(1, sizeof(ht_item));
    if(!item){
        perror("Unable to allocate memory for ht_item\n");
        return NULL;
    }
    item->key = NULL;
    return item;
}

void _ht_item_delete(ht_item *hitem){
    if(hitem){
        if(hitem->key){
            free(hitem->key);
        }
        free(hitem);
    }
}

int _ht_item_print(ht_item *hitem){
    if(hitem){
        if(hitem->key){
            return printf("(%s:%d)", hitem->key, hitem->value);
        }
    }
    return 0;
}

ht_arr * _ht_arr_create(size_t chain_base_sz, size_t dyn_resz){
    ht_arr *harr=NULL;

    harr = (ht_arr *) calloc(1, sizeof(ht_arr));
    if(!harr){
        perror("Unable to allocate memory for ht item array");
        return NULL;
    }
    harr->chain_base_sz = chain_base_sz;
    harr->curr_sz = chain_base_sz;
    harr->insert_idx = 0;
    harr->dyn_resz = dyn_resz;

    harr->item_arr = (ht_item **) calloc(chain_base_sz, sizeof(ht_item *));
    if(!(harr->item_arr)){
        perror("Unable to allocate memory for ht_item array in a ht_arr object");
        return NULL;
    }

    /** At insertion */
    /*
    for(int i=0; i<base_sz; i++){
        if(!(harr->item_arr[i] = _ht_item_create())){
            printf("Unable to allocate memory for ht_item in array index %d\n", i);
            return NULL;
        }
    }
    */

    return harr;
}

void _ht_arr_delete(ht_arr *harr){
    if(harr){
        if(harr->item_arr){
            for(int i=0; i<(harr->curr_sz); i++){
                if(harr->item_arr[i]) _ht_item_delete(harr->item_arr[i]);
            }
            free(harr->item_arr);
        }
        free(harr);
    }
}

int _ht_arr_print(ht_arr *harr, int i){
    int chars=0;
    if(harr){
        if(harr->item_arr){
            printf("[%d] => ", i);
            for(int i=0; i<(harr->insert_idx); i++){
                chars += _ht_item_print(harr->item_arr[i]);
                printf(" ");
            }
        }
    }
    return chars;
}

hash_tb * hash_tb_create(size_t ht_sz, size_t chain_sz, size_t dyn_resz){
    hash_tb *ht=NULL;

    if(ht_sz <= 0 || chain_sz <= 0){
        perror("Hash table size too low");
        return NULL;
    }

    ht = (hash_tb *) calloc(1, sizeof(hash_tb));
    if(!ht){
        perror("Unable to allocate memory for hash table");
        return NULL;
    }
    ht->ht_sz = ht_sz;

    ht->ht_arr = (ht_arr **) calloc(ht_sz, sizeof(ht_arr *));
    if(!(ht->ht_arr)){
        perror("Unable to allocate memory for ht array");
        return NULL;
    }

    for(int i=0; i<ht_sz; i++){
        if(!(ht->ht_arr[i] = _ht_arr_create(chain_sz, dyn_resz))){
            printf("Unable to allocate memory for ht array in hash table index %d\n", i);
            return NULL;
        }
    }

    return ht;
}

void hash_tb_delete(hash_tb *ht){
    if(ht){
        if(ht->ht_arr){
            for(int i=0; i<ht->ht_sz; i++){
                if(ht->ht_arr[i]) _ht_arr_delete(ht->ht_arr[i]);
            }
            free(ht->ht_arr);
        }
        free(ht);
    }
}

int hash_tb_print(hash_tb *ht){
    int chars=0;
    if(ht){
        if(ht->ht_arr){
            for(int i=0; i<(ht->ht_sz); i++){
                chars += _ht_arr_print(ht->ht_arr[i], i);
                printf("\n");
            }
        }
    }
    return chars;
}


int _ht_arr_insert(ht_arr *harr, char *key, int value){
    if(!harr){
        perror("hash table array NULL pointer when inserting");
        return -1;
    }

    if((harr->insert_idx == harr->curr_sz) && !(harr->dyn_resz)){
        return -1;
    }

    if(!(harr->item_arr[harr->insert_idx] = _ht_item_create())){
        perror("Unable to allocate memory for a new ht_item when inserting");
        return -1;
    }

    harr->item_arr[harr->insert_idx]->key = (char *) calloc(strlen(key)+1, sizeof(char));
    strcpy(harr->item_arr[harr->insert_idx]->key, key);

    harr->item_arr[harr->insert_idx]->value = value;

    harr->insert_idx++;
    if(harr->insert_idx == harr->curr_sz && harr->dyn_resz){
        harr->curr_sz += harr->chain_base_sz;
        harr->item_arr = realloc(harr->item_arr, harr->curr_sz);
    }
    return 0;
}

int hash_tb_insert(hash_tb *ht, char *key, int value){
    int hashcode;

    if(!ht){
        perror("Hash table NULL pointer when inserting");
        return -1;
    }
    if(!key){
        perror("key NULL pointer when inserting");
        return -1;
    }

    hashcode = _hash_code(key, ht->ht_sz);
    if(hashcode < 0){
        perror("hashcode invalid value when inserting");
        return -1;
    }

    if(_ht_arr_insert(ht->ht_arr[hashcode], key, value)){
        printf("Unable to insert (%s - %d) into the hash table with hashcode %d\n", key, value, hashcode);
        return -1;
    }

    return 0;
}
