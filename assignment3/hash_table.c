#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

typedef struct _ht_item{
    char *key;
    int value;
}ht_item;

typedef struct _ht_arr{
  ht_item** item_arr;
  size_t base_sz;
  size_t curr_sz;
} ht_arr;

struct _hash_tb{
  ht_arr **ht_arr;
  size_t ht_sz;
};


ht_item * _ht_item_create(){
  ht_item *item=NULL;
  item = (ht_item *) calloc(1, sizeof(ht_item));
  printf("Hdhsajdha");
  return item;
}

void _ht_item_delete(ht_item *hitem){
  if(hitem){
    if(hitem->key) free(hitem->key);
    free(hitem);
  }
}

ht_arr * _ht_arr_create(size_t base_sz){
  ht_arr *harr=NULL;

  harr = (ht_arr *) calloc(1, sizeof(ht_arr));
  harr->base_sz = base_sz;
  harr->curr_sz = base_sz;

  for(int i=0; i<base_sz; i++){
    harr->item_arr[i] = _ht_item_create();
  }

  return harr;
}

void _ht_arr_delete(ht_arr *harr){
  if(harr){
    for(int i=0; i<(harr->curr_sz); i++){
      if(harr->item_arr[i]) _ht_item_delete(harr->item_arr[i]);
    }
    free(harr);
  }
}


hash_tb * hash_tb_create(int ht_sz){
  hash_tb *ht=NULL;

  if(ht_sz <= 0){
    perror("Hash table size too low");
    return NULL;
  }

  ht = (hash_tb *) calloc(1, sizeof(hash_tb));
  ht->ht_sz = ht_sz;

  ht->ht_arr = (ht_arr **) calloc(ht_sz, sizeof(ht_arr *));

  for(int i=0; i<ht_sz; i++){
    ht->ht_arr[i] = _ht_arr_create(ht_sz);
  }

  return ht;
}


void hash_tb_delete(hash_tb *ht){
  if(ht){
    for(int i=0; i<ht->ht_sz; i++){
      if(ht->ht_arr[i]) _ht_arr_delete(ht->ht_arr[i]);
    }
    free(ht);
  }
}
