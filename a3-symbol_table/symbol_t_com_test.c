#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table_com.h"
#include "symbol_table_com.h"

#define SYMB_ID "IDENTIFIER_ID"

int main(){
    symbol_tb_com *st=NULL;
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

    printf("\n···> Creating Symbol table...\n");
    st = symb_tb_com_create();
    if(!st){
        printf("Unable to allocate memory for a symbol table\n");
        return -1;
    }
    printf("···> Created!\n\n");

    /************************************************************************/
    printf("···> Inserting param in global table...\n");
    if(symb_tb_com_insert(st, symb)){
        printf("Insertion in global table FAILED\n");
        return -1;
    }
    printf("Inserted!\n\n");

    int ret;
    printf("···> Searching introduced key in global table...\n");
    ret = symb_tb_com_isKey(st, symb->id);
    if (ret == 1) printf("Found!\n\n");
    else if (ret == 0){
        printf("Not found![WRONG OUTPUT]\n\n");
        return -1;
    }
    else{
        printf("Search in global table FAILED\n");
        return -1;
    }

    printf("···> Searching non-introduced key in global table...\n");
    ret = symb_tb_com_isKey(st, "ANOTHER_ID");
    if (ret == 1){
        printf("Found![WRONG OUTPUT]\n\n");
        return -1;
    }
    else if (ret == 0) printf("Not found!\n\n");
    else {
        printf("Search in global table FAILED\n");
        return -1;
    }
    /************************************************************************/

    /************************************************************************/
    symb->symb_cat = FUNCTION;
    strcpy(symb->id, "FUNCTION1");
    printf("···> Inserting new domain...\n");
    if(symb_tb_com_insert(st, symb)){
        printf("Insertion of new domain FAILED\n");
        return -1;
    }
    printf("Inserted!\n\n");

    symb->symb_cat = 0;
    strcpy(symb->id, "ID2");
    printf("···> Inserting param in local table...\n");
    if(symb_tb_com_insert(st, symb)){
        printf("Insertion in local table FAILED\n");
        return -1;
    }
    printf("Inserted!\n\n");

    printf("···> Searching introduced key in local table...\n");
    ret = symb_tb_com_isKey(st, "ID2");
    if (ret == 1) printf("Found!\n\n");
    else if (ret == 0) {
        printf("Not found![WRONG OUTPUT]\n\n");
        return -1;
    }
    else {
        printf("Search in local table FAILED\n");
        return -1;
    }

    printf("···> Searching non-introduced key in local table...\n");
    ret = symb_tb_com_isKey(st, "NO_ID2");
    if (ret == 1) {
        printf("Found![WRONG OUTPUT]\n\n");
        return -1;
    }
    else if (ret == 0) printf("Not found!\n\n");
    else {
        printf("Search in local table FAILED\n");
        return -1;
    }

    printf("···> Searching non-introduced key in local table but introduced in global table...\n");
    ret = symb_tb_com_isKey(st, SYMB_ID);
    if (ret == 1) printf("Found!\n\n");
    else if (ret == 0) {
        printf("Not found![WRONG OUTPUT]\n\n");
        return -1;
    }
    else {
        printf("Search in global table FAILED\n");
        return -1;
    }
    /************************************************************************/

    /************************************************************************/
    strcpy(symb->id, "FUNCTION1");
    printf("···> Inserting funcion1 in local table...\n");
    if(!symb_tb_com_insert(st, symb)){
        printf("Inserted! (WRONG CASE)\n");
        return -1;
    }
    printf("Insertion in local table FAILED (GOOD CASE)\n\n");
    /************************************************************************/

    /************************************************************************/
    strcpy(symb->id, "cierre");
    symb->symb_cat = FUNCTION;
    printf("···> Deleting local domain...\n");
    if(symb_tb_com_insert(st, symb)){
        printf("Deletion of local domain FAILED\n");
        return -1;
    }
    printf("Domain deleted!\n\n");

    printf("···> Searching introduced key in local table...\n");
    ret = symb_tb_com_isKey(st, "ID2");
    if (ret == 1) {
        printf("Found! (WRONG CASE)\n");
        return -1;
    }
    else if (ret == 0) printf("Not found!\n");
    else {
        printf("Search in local table FAILED\n");
        return -1;
    }
    /************************************************************************/

    /************************************************************************/
    printf("\n···> Deleting Symbol table...\n");
    symb_tb_com_delete(st);
    printf("···> Deleted!\n\n");

    free(symb->id);
    free(symb);

    return 0;
}
