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

// struct _symbol_tb{
//     hash_tb *global;
//     hash_tb *local[MAX_LOCAL_TB];
//     int current_local_tb;
// };


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

    sym_tb->current_local_tb = -1;

    return sym_tb;
}


void symb_tb_delete(symbol_tb *symb_tb){
    if(symb_tb){
        if(symb_tb->global) hash_tb_delete(symb_tb->global);
        for(int i=0; i<=symb_tb->current_local_tb; i++){
            if(symb_tb->local[i]) hash_tb_delete(symb_tb->local[i]);
        }
        free(symb_tb);
    }
}


int symb_tb_insert(symbol_tb *symb_tb, char *key, int value){
    if(!symb_tb){
        perror("Symbol table NULL pointer when inserting");
        return -1;
    }

    //DEBUG
    printf("=== INSERTING (%s : %d) [current_local_tb %d]\n", key, value, symb_tb->current_local_tb);

    /* New domain */
    if(value < 0 && strcmp(CLOSURE_STR, key)){
        //DEBUG
        printf("\t== Inserting NEW DOMAIN (%s : %d)\n", key, value);
        if(hash_tb_isKey(symb_tb->global, key) == 1){
            /* The domain already exists at the global table */
            return -1;
        }
        for(int i=0; i<=symb_tb->current_local_tb; i++){
            if(hash_tb_isKey(symb_tb->local[i], key) == 1){
                /* The domain already exists at one of the created local tables */
                return -1;
            }
        }
        if(symb_tb->current_local_tb == MAX_LOCAL_TB-1){
            /* Unable to create a new domain because the limit is already reached */
            return -1;
        }
        /* Inserting in global table */
        hash_tb_insert(symb_tb->global, key, value);
        /* Inserting in local tables */
        for(int j=0; j<=symb_tb->current_local_tb; j++){
            hash_tb_insert(symb_tb->local[j], key, value);
        }
        symb_tb->current_local_tb++; /* New domain index */
        /* Creating new local table */
        symb_tb->local[symb_tb->current_local_tb] = hash_tb_create(HASH_TB_SIZE, HASH_TB_CHAIN_SIZE, HT_DYN_RESZ_BOOL);
        /* Inserting in the new local table */
        hash_tb_insert(symb_tb->local[symb_tb->current_local_tb], key, value);
    }
    /* Closing domain */
    else if(value == CLOSURE_CONST && !strcmp(CLOSURE_STR, key)){
        //DEBUG
        printf("\t== CLOSING domain  (%s : %d)\n", key, value);
        /* Deleting current local table */
        if(symb_tb->local[symb_tb->current_local_tb]){
            hash_tb_delete(symb_tb->local[symb_tb->current_local_tb]);
        }
        symb_tb->local[symb_tb->current_local_tb] = NULL;
        symb_tb->current_local_tb--;
    }
    /* New element */
    else{
        //DEBUG
        printf("\t== Inserting NEW ELEMENT (%s : %d)\n", key, value);
        /* If any local table is initialized */
        if(symb_tb->current_local_tb>=0){
            if(hash_tb_isKey(symb_tb->local[symb_tb->current_local_tb], key) == 1){
                /* The element already exists at the current local table */
                return -1;
            }
            printf("\t\t== Inserting NEW ELEMENT LOCAL (%s : %d)\n", key, value);
            /* Inserting in current local table */
            hash_tb_insert(symb_tb->local[symb_tb->current_local_tb], key, value);
        }
        /* ZERO local table initialized ==>  Trying to insert in global table */
        else{
            if(hash_tb_isKey(symb_tb->global, key) == 1){
                /* The element already exists at the global table */
                return -1;
            }
            printf("\t\t== Inserting NEW ELEMENT GLOBAL (%s : %d)\n", key, value);
            /* Inserting in global table */
            hash_tb_insert(symb_tb->global, key, value);
        }
    }
    return 0;
}


int symb_tb_isKey(symbol_tb *symb_tb, char *key){
    if(!symb_tb){
        perror("Symbol table NULL pointer when searching");
        return -1;
    }
    if(!key){
        perror("Key NULL pointer when searching");
        return -1;
    }

    /* If any local table is not NULL we are in a local domain */
    /* We must search first at local domains */
    if(symb_tb->current_local_tb>=0){
        for(int i=symb_tb->current_local_tb; i>=0; i--){
            /* If the key is found in any local domain, it's no needed to keep searching */
            if(hash_tb_isKey(symb_tb->local[i], key) == 1) return 1;
        }
    }
    /* If local search fails, we must search in global table */
    if(hash_tb_isKey(symb_tb->global, key) == 1) return 1;
    /* Not found (or memory error at hash_tb_isKey funct's) */
    return 0;
}


int symb_tb_search(symbol_tb *symb_tb, char *key, int *value){
    if(!symb_tb){
        perror("Symbol table NULL pointer when searching");
        return -1;
    }
    if(!key){
        perror("Key NULL pointer when searching");
        return -1;
    }

    /* If any local table is not NULL we are in a local domain */
    /* We must search first at local domains */
    if(symb_tb->current_local_tb>=0){
        for(int i=symb_tb->current_local_tb; i>=0; i--){
            /* If the key is found in any local domain, it's no needed to keep searching */
            if(hash_tb_get(symb_tb->local[i], key, value) == 1) return 0;
        }
    }
    /* If local search fails, we must search in global table */
    if(hash_tb_get(symb_tb->global, key, value) == 1) return 0;
    /* Not found (or memory error at hash_tb_isKey funct's) */
    return -1;
}
