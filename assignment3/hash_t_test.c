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

  printf("Inserting one element...\n");
  if(hash_tb_insert(ht, "Francesco Virgolini", 45)){
      perror("Unable to insert into the hash table");
      return -1;
  }
  printf("Inserted!\n");

  printf("Printing hash table...\n");
  hash_tb_print(ht);
  printf("Printed!\n");

  printf("Deleting Hash table...\n");
  hash_tb_delete(ht);
  printf("Deleted!\n");

  return 0;
}
