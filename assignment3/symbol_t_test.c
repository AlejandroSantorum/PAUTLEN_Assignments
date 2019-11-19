#include <stdio.h>
#include <stdlib.h>

#include "hash_table.h"
#include "symbol_table.h"


int main(){
    symbol_tb *st=NULL;

    printf("\n···> Creating Symbol table...\n");
    st = symb_tb_create();
    if(!st){
        printf("Unable to allocate memory for a symbol table\n");
        return -1;
    }
    printf("···> Created!\n\n");

    /************************************************************************/
    printf("···> Inserting param in global table...\n");
    if(symb_tb_insert(st, "uno", 1)){
        printf("Insertion in global table FAILED\n");
        return -1;
    }
    printf("Inserted!\n\n");

    printf("···> Searching introduced key in global table...\n");
    if(symb_tb_search(st, "uno")){
        printf("Search in global table FAILED\n");
        return -1;
    }
    printf("Found!\n\n");

    printf("···> Searching non-introduced key in global table...\n");
    if(!symb_tb_search(st, "dos")){
        printf("Search in global table FAILED\n");
        return -1;
    }
    printf("Not Found!\n\n");
    /************************************************************************/

    /************************************************************************/
    printf("···> Inserting new domain...\n");
    if(symb_tb_insert(st, "funcion1", -10)){
        printf("Insertion of new domain FAILED\n");
        return -1;
    }
    printf("Inserted!\n\n");

    printf("···> Inserting param in local table...\n");
    if(symb_tb_insert(st, "dos", 2)){
        printf("Insertion in local table FAILED\n");
        return -1;
    }
    printf("Inserted!\n\n");

    printf("···> Searching introduced key in local table...\n");
    if(symb_tb_search(st, "dos")){
        printf("Search in local table FAILED\n");
        return -1;
    }
    printf("Found!\n\n");

    printf("···> Searching non-introduced key in local table...\n");
    if(!symb_tb_search(st, "tres")){
        printf("Search in local table FAILED\n");
        return -1;
    }
    printf("Not Found!\n\n");

    printf("···> Searching non-introduced key in local table but introduced in global table...\n");
    if(symb_tb_search(st, "uno")){
        printf("Search in global table FAILED\n");
        return -1;
    }
    printf("Found!\n\n");
    /************************************************************************/

    /************************************************************************/
    printf("···> Inserting funcion1 in local table...\n");
    if(!symb_tb_insert(st, "funcion1", 10)){
        printf("Inserted! (WRONG CASE)\n");
        return -1;
    }
    printf("Insertion in local table FAILED (GOOD CASE)\n\n");
    /************************************************************************/

    /************************************************************************/
    printf("···> Deleting local domain...\n");
    if(symb_tb_insert(st, "cierre", -999)){
        printf("Deletion of local domain FAILED\n");
        return -1;
    }
    printf("Domain deleted!\n\n");

    printf("···> Searching introduced key in local table...\n");
    if(!symb_tb_search(st, "dos")){
        printf("Found! (WRONG CASE)\n");
        return -1;
    }
    printf("Search in local table FAILED (GOOD CASE!)\n\n");
    /************************************************************************/

    /************************************************************************/
    printf("\n···> Deleting Symbol table...\n");
    symb_tb_delete(st);
    printf("···> Deleted!\n\n");

    return 0;
}
