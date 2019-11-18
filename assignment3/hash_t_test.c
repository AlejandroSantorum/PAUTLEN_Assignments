#include <stdio.h>
#include <stdlib.h>

#include "hash_table.h"


int main(){
  hash_tb *ht=NULL;

  ht = hash_tb_create(16);
  if(!ht){
    printf("Memory Error: Unable to allocate memory for the hash table\n");
    return -1;
  }

  hash_tb_delete(ht);

  return 0;
}
