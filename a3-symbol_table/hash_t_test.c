#include <stdio.h>
#include <stdlib.h>

#include "hash_table.h"


int main(){
  hash_tb *ht=NULL;

  printf("\n···> Creating Hash table...\n");
  ht = hash_tb_create(16, 8, 1);
  if(!ht){
    printf("Memory Error: Unable to allocate memory for the hash table\n");
    return -1;
  }
  printf("···> Created!\n\n");

  printf("···> Inserting one element...\n");
  if(hash_tb_insert(ht, "Francesco Virgolini", 45)){
      perror("Unable to insert into the hash table");
      return -1;
  }
  printf("···> Inserted!\n\n");

  printf("···> Printing hash table...\n");
  hash_tb_print(ht);
  printf("···> Printed!\n\n");

  printf("···> Checking presence of introduced key...\n");
  int ret = hash_tb_isKey(ht, "Francesco Virgolini");
  if(ret == -1) printf("Error when checking key [WRONG outcome]\n");
  else if(ret == 0) printf("Key is NOT in the hash table [WRONG outcome]\n");
  else if(ret == 1) printf("Key IS IN the hash table [GOOD outcome]\n");

  int val;
  printf("···> Getting value of introduced key...\n");
  ret = hash_tb_get(ht, "Francesco Virgolini", &val);
  if(ret == -1) printf("Error when getting key value[WRONG outcome]\n");
  else if(ret == 0) printf("Key is NOT in the hash table [WRONG outcome]\n");
  else if(ret == 1) printf("Key IS IN the hash table [GOOD outcome] and the value is %d\n", val);

  printf("···> Checking presence of non-introduced key...\n");
  ret = hash_tb_isKey(ht, "Francesco Virgolini 2");
  if(ret == -1) printf("Error when checking key [WRONG outcome]\n");
  else if(ret == 0) printf("Key is NOT in the hash table [GOOD outcome]\n");
  else if(ret == 1) printf("Key IS IN the hash table [WRONG outcome]\n");

  printf("···> Getting value of non-introduced key...\n");
  ret = hash_tb_get(ht, "Francesco Virgolini 2", &val);
  if(ret == -1) printf("Error when getting key value[WRONG outcome]\n");
  else if(ret == 0) printf("Key is NOT in the hash table [GOOD outcome]\n");
  else if(ret == 1) printf("Key IS IN the hash table [WRONG outcome] and the value is %d\n", val);

  printf("\n···> Deleting Hash table...\n");
  hash_tb_delete(ht);
  printf("···> Deleted!\n\n");

  /******************************************************/

  printf(">>> CHECKING REALLOC <<<\n");
  printf("\n···> Creating Hash table...\n");
  ht = hash_tb_create(4, 2, 1);
  if(!ht){
    printf("Memory Error: Unable to allocate memory for the hash table\n");
    return -1;
  }
  printf("···> Created!\n\n");

  printf("···> Inserting element 1...\n");
  if(hash_tb_insert(ht, "Francesco Virgolini 1", 1)){
      perror("Unable to insert into the hash table");
      return -1;
  }
  printf("···> Inserted!\n\n");

  printf("···> Inserting element 2... (now it reallocs)\n");
  if(hash_tb_insert(ht, "Francesco Virgolini 2", 2)){
      perror("Unable to insert into the hash table");
      return -1;
  }
  printf("···> Inserted!\n\n");

  printf("···> Inserting element 3... (now it is inserted in realloced position)\n");
  if(hash_tb_insert(ht, "Francesco Virgolini 3", 3)){
      perror("Unable to insert into the hash table");
      return -1;
  }
  printf("···> Inserted!\n\n");

  printf("···> Inserting element 4... (now it is inserted in realloced position) [reallocing again]\n");
  if(hash_tb_insert(ht, "Francesco Virgolini 4", 4)){
      perror("Unable to insert into the hash table");
      return -1;
  }
  printf("···> Inserted!\n\n");

  printf("···> Inserting element 5... (now it is inserted in realloced position x2)\n");
  if(hash_tb_insert(ht, "Francesco Virgolini 5", 5)){
      perror("Unable to insert into the hash table");
      return -1;
  }
  printf("···> Inserted!\n\n");

  printf("···> Getting value of first introduced key...\n");
  ret = hash_tb_get(ht, "Francesco Virgolini 1", &val);
  if(ret == -1) printf("Error when getting key value[WRONG outcome]\n");
  else if(ret == 0) printf("Key is NOT in the hash table [WRONG outcome]\n");
  else if(ret == 1) printf("Key IS IN the hash table [GOOD outcome] and the value is %d\n", val);

  printf("···> Getting value of third introduced key (realloced one)...\n");
  ret = hash_tb_get(ht, "Francesco Virgolini 3", &val);
  if(ret == -1) printf("Error when getting key value[WRONG outcome]\n");
  else if(ret == 0) printf("Key is NOT in the hash table [WRONG outcome]\n");
  else if(ret == 1) printf("Key IS IN the hash table [GOOD outcome] and the value is %d\n", val);

  printf("···> Getting value of 4th introduced key (realloced one)...\n");
  ret = hash_tb_get(ht, "Francesco Virgolini 4", &val);
  if(ret == -1) printf("Error when getting key value[WRONG outcome]\n");
  else if(ret == 0) printf("Key is NOT in the hash table [WRONG outcome]\n");
  else if(ret == 1) printf("Key IS IN the hash table [GOOD outcome] and the value is %d\n", val);

  printf("···> Getting value of 5th introduced key (realloced one x2)...\n");
  ret = hash_tb_get(ht, "Francesco Virgolini 5", &val);
  if(ret == -1) printf("Error when getting key value[WRONG outcome]\n");
  else if(ret == 0) printf("Key is NOT in the hash table [WRONG outcome]\n");
  else if(ret == 1) printf("Key IS IN the hash table [GOOD outcome] and the value is %d\n", val);

  printf("···> Printing hash table...\n");
  hash_tb_print(ht);
  printf("···> Printed!\n\n");

  printf("\n···> Deleting Hash table...\n");
  hash_tb_delete(ht);
  printf("···> Deleted!\n\n");

  return 0;
}
