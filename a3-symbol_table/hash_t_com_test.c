#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table_com.h"


#define SYMB_ID "IDENTIFIER_ID"


int main(){
    hash_tb_com *ht=NULL;

    Symbol *symb=NULL;

    symb = (Symbol *) calloc(1, sizeof(Symbol));
    symb->id = (char *) calloc(sizeof(SYMB_ID), sizeof(char));
    strcpy(symb->id, SYMB_ID);
    symb->symb_cat = 0;
    symb->symb_type = 0;
    symb->var_cat = 0;
    symb->value = 0;
    symb->len = 0;
    symb->num_param = 0;
    symb->pos = 0;
    symb->num_local_var = 0;

    printf("\n···> Creating Hash table...\n");
    ht = hash_tb_com_create(16, 8, 1);
    if(!ht){
    printf("Memory Error: Unable to allocate memory for the hash table\n");
    return -1;
    }
    printf("···> Created!\n\n");

    printf("···> Inserting one element...\n");
    if(hash_tb_com_insert(ht, symb)){
      perror("Unable to insert into the hash table");
      return -1;
    }
    printf("···> Inserted!\n\n");

    printf("···> Printing hash table...\n");
    hash_tb_com_print(ht);
    printf("···> Printed!\n\n");

    printf("···> Checking presence of introduced key...\n");
    int ret = hash_tb_com_isKey(ht, SYMB_ID);
    if(ret == -1) printf("Error when checking key [WRONG outcome]\n");
    else if(ret == 0) printf("Key is NOT in the hash table [WRONG outcome]\n");
    else if(ret == 1) printf("Key IS IN the hash table [GOOD outcome]\n");

    Symbol *val;
    printf("···> Getting value of introduced key...\n");
    ret = hash_tb_com_get(ht, SYMB_ID, &val);
    if(ret == -1) printf("Error when getting key value[WRONG outcome]\n");
    else if(ret == 0) printf("Key is NOT in the hash table [WRONG outcome]\n");
    else if(ret == 1) printf("Key IS IN the hash table [GOOD outcome] and the value is %d\n", val->value);

    printf("···> Checking presence of non-introduced key...\n");
    ret = hash_tb_com_isKey(ht, "Francesco Virgolini 2");
    if(ret == -1) printf("Error when checking key [WRONG outcome]\n");
    else if(ret == 0) printf("Key is NOT in the hash table [GOOD outcome]\n");
    else if(ret == 1) printf("Key IS IN the hash table [WRONG outcome]\n");

    printf("···> Getting value of non-introduced key...\n");
    ret = hash_tb_com_get(ht, "Francesco Virgolini 2", &val);
    if(ret == -1) printf("Error when getting key value[WRONG outcome]\n");
    else if(ret == 0) printf("Key is NOT in the hash table [GOOD outcome]\n");
    else if(ret == 1) printf("Key IS IN the hash table [WRONG outcome] and the value is %d\n", val->value);

    printf("\n···> Deleting Hash table...\n");
    hash_tb_com_delete(ht);
    printf("···> Deleted!\n\n");

  /******************************************************/

    printf(">>> CHECKING REALLOC <<<\n");
    printf("\n···> Creating Hash table...\n");
    ht = hash_tb_com_create(4, 2, 1);
    if(!ht){
    printf("Memory Error: Unable to allocate memory for the hash table\n");
    return -1;
    }
    printf("···> Created!\n\n");

    printf("···> Inserting element 1...\n");
    strcpy(symb->id, "IDENTIFIER_1");
    symb->value = 1;
    if(hash_tb_com_insert(ht, symb)){
      perror("Unable to insert into the hash table");
      return -1;
    }
    printf("···> Inserted!\n\n");

    printf("···> Inserting element 2... (now it reallocs)\n");
    strcpy(symb->id, "IDENTIFIER_2");
    symb->value = 2;
    if(hash_tb_com_insert(ht, symb)){
      perror("Unable to insert into the hash table");
      return -1;
    }
    printf("···> Inserted!\n\n");

    printf("···> Inserting element 3... (now it is inserted in realloced position)\n");
    strcpy(symb->id, "IDENTIFIER_3");
    symb->value = 3;
    if(hash_tb_com_insert(ht, symb)){
      perror("Unable to insert into the hash table");
      return -1;
    }
    printf("···> Inserted!\n\n");

    printf("···> Inserting element 4... (now it is inserted in realloced position) [reallocing again]\n");
    strcpy(symb->id, "IDENTIFIER_4");
    symb->value = 4;
    if(hash_tb_com_insert(ht, symb)){
      perror("Unable to insert into the hash table");
      return -1;
    }
    printf("···> Inserted!\n\n");

    printf("···> Inserting element 5... (now it is inserted in realloced position x2)\n");
    strcpy(symb->id, "IDENTIFIER_5");
    symb->value = 5;
    if(hash_tb_com_insert(ht, symb)){
      perror("Unable to insert into the hash table");
      return -1;
    }
    printf("···> Inserted!\n\n");

    printf("···> Getting value of first introduced key...\n");
    ret = hash_tb_com_get(ht, "IDENTIFIER_1", &val);
    if(ret == -1) printf("Error when getting key value[WRONG outcome]\n");
    else if(ret == 0) printf("Key is NOT in the hash table [WRONG outcome]\n");
    else if(ret == 1) printf("Key IS IN the hash table [GOOD outcome] and the value is %d\n", val->value);

    printf("···> Getting value of third introduced key (realloced one)...\n");
    ret = hash_tb_com_get(ht, "IDENTIFIER_3", &val);
    if(ret == -1) printf("Error when getting key value[WRONG outcome]\n");
    else if(ret == 0) printf("Key is NOT in the hash table [WRONG outcome]\n");
    else if(ret == 1) printf("Key IS IN the hash table [GOOD outcome] and the value is %d\n", val->value);

    printf("···> Getting value of 4th introduced key (realloced one)...\n");
    ret = hash_tb_com_get(ht, "IDENTIFIER_4", &val);
    if(ret == -1) printf("Error when getting key value[WRONG outcome]\n");
    else if(ret == 0) printf("Key is NOT in the hash table [WRONG outcome]\n");
    else if(ret == 1) printf("Key IS IN the hash table [GOOD outcome] and the value is %d\n", val->value);

    printf("···> Getting value of 5th introduced key (realloced one x2)...\n");
    ret = hash_tb_com_get(ht, "IDENTIFIER_5", &val);
    if(ret == -1) printf("Error when getting key value[WRONG outcome]\n");
    else if(ret == 0) printf("Key is NOT in the hash table [WRONG outcome]\n");
    else if(ret == 1) printf("Key IS IN the hash table [GOOD outcome] and the value is %d\n", val->value);

    printf("···> Printing hash table...\n");
    hash_tb_com_print(ht);
    printf("···> Printed!\n\n");

    printf("\n···> Deleting Hash table...\n");
    hash_tb_com_delete(ht);
    printf("···> Deleted!\n\n");

  return 0;
}