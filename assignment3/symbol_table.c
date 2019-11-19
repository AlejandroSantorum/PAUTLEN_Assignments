#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbol_table.h"
#include "hash_table.h"

#define HASH_TB_SIZE 32
#define HASH_TB_CHAIN_SIZE 4
#define HT_DYN_RESZ_BOOL 1

#define CLOSURE_STR "cierre"
#define CLOSURE_CONST -999

struct _symbol_tb{
    hash_tb *global;
    hash_tb *local;
};


symbol_tb * symb_tb_create(){
    symbol_tb *sym_tb;

    sym_tb = calloc(1, sizeof(sym_tb));
    if(!sym_tb){
        perror("Unable to allocate memory for symbol table");
        return NULL;
    }

    sym_tb->global = hash_tb_create(HASH_TB_SIZE, HASH_TB_CHAIN_SIZE, HT_DYN_RESZ_BOOL);
    if(!(sym_tb->global)){
        printf("Memory Error: Unable to allocate memory for the global hash table\n");
        return NULL;
    }

    sym_tb->local = NULL;

    return sym_tb;
}


void symb_tb_delete(symbol_tb *symb_tb){
    if(symb_tb){
        if(symb_tb->global) hash_tb_delete(symb_tb->global);
        if(symb_tb->local) hash_tb_delete(symb_tb->local);
        free(symb_tb);
    }
}


int symb_tb_insert(symbol_tb *symb_tb, char *key, int value){
    if(!symb_tb){
        perror("Symbol table NULL pointer when inserting");
        return -1;
    }

    /* New domain */
    if(value < 0 && strcmp(CLOSURE_STR, key)){
        if(hash_tb_isKey(symb_tb->global, key) == 1){
            /* The domain already exists at the global table */
            return -1;
        }
        /* Inserting in global table */
        hash_tb_insert(symb_tb->global, key, value);
        /* Creating local table */
        symb_tb->local = hash_tb_create(HASH_TB_SIZE, HASH_TB_CHAIN_SIZE, HT_DYN_RESZ_BOOL);
        /* Inserting in local table */
        hash_tb_insert(symb_tb->local, key, value);
    }
    /* Closing domain */
    else if(value == CLOSURE_CONST && !strcmp(CLOSURE_STR, key)){
        /* Deleting local table */
        if(symb_tb->local) hash_tb_delete(symb_tb->local);
        symb_tb->local = NULL;
    }
    /* New element */
    else{
        /* Local table initialized */
        if(symb_tb->local){
            if(hash_tb_isKey(symb_tb->local, key) == 1){
                /* The element already exists at the local table */
                return -1;
            }
            /* Inserting in local table */
            hash_tb_insert(symb_tb->local, key, value);
        }
        /* Local table NOT initialized ==>  Trying to insert in global table */
        else{
            if(hash_tb_isKey(symb_tb->global, key) == 1){
                /* The element already exists at the global table */
                return -1;
            }
            /* Inserting in global table */
            hash_tb_insert(symb_tb->global, key, value);
        }
    }
    return 0;
}


int symb_tb_search(symbol_tb *symb_tb, char *key){
    if(!symb_tb){
        perror("Symbol table NULL pointer when searching");
        return -1;
    }
    if(!key){
        perror("Key NULL pointer when searching");
        return -1;
    }

    /* If local table is not NULL we are in a local domain */
    /* We must search first at local domain */
    if(symb_tb->local){
        /* If the key is found in local domain, it's no needed to keep searching */
        if(hash_tb_isKey(symb_tb->local, key) == 1) return 0;
    }
    /* If local search fails, we must search in global table */
    if(hash_tb_isKey(symb_tb->global, key) == 1) return 0;
    /* Not found (or memory error at hash_tb_isKey funct's) */
    return -1;
}
