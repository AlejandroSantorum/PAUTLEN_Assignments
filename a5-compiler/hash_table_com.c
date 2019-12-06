#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table_com.h"

#define HASHCODE_LEN 3


typedef struct _ht_arr{
    Symbol** symb_arr;
    size_t chain_base_sz;
    size_t curr_sz;
    size_t insert_idx;
    size_t dyn_resz;
} ht_arr;

struct _hash_tb_com{
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


Symbol * _symbol_copy(Symbol *src){
    Symbol *symb=NULL;

    symb = (Symbol *) calloc(1, sizeof(Symbol));
    if(!symb){
        perror("Unable to allocate memory for symbol\n");
        return NULL;
    }

    symb->id = (char *) calloc(strlen(src->id)+1, sizeof(char));
    if(!symb->id){
        perror("Unable to allocate memory for symbol id\n");
        return NULL;
    }

    strcpy(symb->id, src->id);

    symb->symb_cat = src->symb_cat;
    symb->symb_type = src->symb_type;
    symb->var_cat = src->var_cat;
    symb->value = src->value;
    symb->len = src->len;
    symb->num_param = src->num_param;
    symb->pos = src->pos;
    symb->num_local_var = src->num_local_var;

    return symb;
}

void _symbol_delete(Symbol *symb){
    if (symb){
        if (symb->id) free(symb->id);
        free(symb);
    }
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

    harr->symb_arr = (Symbol **) calloc(chain_base_sz, sizeof(Symbol *));
    if(!(harr->symb_arr)){
        perror("Unable to allocate memory for symbol array in a ht_arr object");
        return NULL;
    }

    return harr;
}

void _ht_arr_delete(ht_arr *harr){
    if(harr){
        if(harr->symb_arr){
            for(int i=0; i<harr->insert_idx; i++){
                if(harr->symb_arr[i]){
                    _symbol_delete(harr->symb_arr[i]);
                }
            }
            free(harr->symb_arr);
        }
        free(harr);
    }
}


hash_tb_com * hash_tb_com_create(size_t ht_sz, size_t chain_sz, size_t dyn_resz){
    hash_tb_com *ht=NULL;

    if(ht_sz <= 0 || chain_sz <= 0){
        perror("Hash table size too low");
        return NULL;
    }

    ht = (hash_tb_com *) calloc(1, sizeof(hash_tb_com));
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

void hash_tb_com_delete(hash_tb_com *ht){
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


int _ht_arr_insert(ht_arr *harr, Symbol *symb){
    if(!harr){
        perror("hash table array NULL pointer when inserting");
        return -1;
    }
    /* Checking if there is space left */
    if((harr->insert_idx == harr->curr_sz) && !(harr->dyn_resz)){
        return -1;
    }

    /* Copying symbol and inserting it */
    if(!(harr->symb_arr[harr->insert_idx] = _symbol_copy(symb))){
        perror("Unable to allocate memory for a new symbol when inserting");
        return -1;
    }

    /* Replacing inserting index and reallocing if necessary */
    harr->insert_idx++;
    if((harr->insert_idx == harr->curr_sz) && harr->dyn_resz){
        harr->curr_sz *= 2;
        harr->symb_arr = realloc(harr->symb_arr, harr->curr_sz * sizeof(Symbol*));
        if(!harr->symb_arr){
            perror("Unable to Reallocate memory for a new ht_arr when inserting");
            return -1;
        }
    }
    return 0;
}

int hash_tb_com_insert(hash_tb_com *ht, Symbol *symb){
    int hashcode;

    if(!ht){
        perror("Hash table NULL pointer when inserting");
        return -1;
    }
    if(!symb){
        perror("Symbol NULL pointer when inserting");
        return -1;
    }

    hashcode = _hash_code(symb->id, ht->ht_sz);
    if(hashcode < 0){
        perror("hashcode invalid value when inserting");
        return -1;
    }

    if(_ht_arr_insert(ht->ht_arr[hashcode], symb)){
        printf("Unable to insert symbol with id = %s into the hash table with hashcode %d\n", symb->id, hashcode);
        return -1;
    }

    return 0;
}


int _ht_arr_isKey(ht_arr *harr, char *key){
    if(!harr){
        perror("hash table array NULL pointer when checking the key presence");
        return -1;
    }

    for(int i=0; i<(harr->insert_idx); i++){
        if(!strcmp(harr->symb_arr[i]->id, key)) return 1;
    }
    return 0;
}

int hash_tb_com_isKey(hash_tb_com *ht, char *key){
    int hashcode;

    if(!ht){
        perror("Hash table NULL pointer when checking the key presence");
        return -1;
    }
    if(!key){
        perror("key NULL pointer when checking the key presence");
        return -1;
    }

    hashcode = _hash_code(key, ht->ht_sz);

    if(hashcode < 0){
        perror("hashcode invalid value when checking the key presence");
        return -1;
    }

    return _ht_arr_isKey(ht->ht_arr[hashcode], key);
}


Symbol * _ht_arr_get(ht_arr *harr, char *key){
    if(!harr){
        perror("hash table array NULL pointer when getting the key");
        return NULL;
    }

    for(int i=0; i<(harr->insert_idx); i++){
        if(!strcmp(harr->symb_arr[i]->id, key)){
            return _symbol_copy(harr->symb_arr[i]);
        }
    }
    return NULL;
}

Symbol * hash_tb_com_get(hash_tb_com *ht, char *key){
    int hashcode;

    if(!ht){
        perror("Hash table NULL pointer when getting key");
        return NULL;
    }
    if(!key){
        perror("key NULL pointer when getting key");
        return NULL;
    }

    hashcode = _hash_code(key, ht->ht_sz);
    if(hashcode < 0){
        perror("hashcode invalid value when getting key");
        return NULL;
    }

    return _ht_arr_get(ht->ht_arr[hashcode], key);
}


int hash_tb_com_print(hash_tb_com *ht){
    int ret = 0;

    for(int i=0; i<(ht->ht_sz); i++){
        ret += printf("[%d]:\n\t", i);
        for(int j=0; j<(ht->ht_arr[i]->insert_idx); j++){
            ret += printf("(%s) ", ht->ht_arr[i]->symb_arr[j]->id);
        }
        ret += printf("\n");
    }

    return ret;
}
