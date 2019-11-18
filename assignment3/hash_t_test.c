#include <stdio.h>
#include <stdlib.h>

#include "hash_table.h"


int main(){
  hash_tb *ht=NULL;

  printf("Creating Hash table...\n");
  ht = hash_tb_create(16);
  if(!ht){
    printf("Memory Error: Unable to allocate memory for the hash table\n");
    return -1;
  }
  printf("Created!\n");

  printf("Deleting Hash table...\n");
  hash_tb_delete(ht);
  printf("Deleted!\n");

  return 0;
}
