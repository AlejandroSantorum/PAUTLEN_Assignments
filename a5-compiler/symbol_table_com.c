#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbol_table_com.h"
#include "hash_table_com.h"

#define HASH_TB_SIZE 32
#define HASH_TB_CHAIN_SIZE 4
#define HT_DYN_RESZ_BOOL 1

#define CLOSURE_STR "cierre"
#define CLOSURE_CONST -999

struct _symbol_tb_com{
    hash_tb_com *global;
    hash_tb_com *local[MAX_LOCAL_TB];
    int current_local_tb;
};


symbol_tb_com * symb_tb_com_create(){
    symbol_tb_com *sym_tb;

    sym_tb = calloc(1, sizeof(symbol_tb_com));
    if(!sym_tb){
        perror("Unable to allocate memory for symbol table");
        return NULL;
    }

    sym_tb->global = hash_tb_com_create(HASH_TB_SIZE, HASH_TB_CHAIN_SIZE, HT_DYN_RESZ_BOOL);
    if(!(sym_tb->global)){
        printf("Memory Error: Unable to allocate memory for the global hash table\n");
        return NULL;
    }

    sym_tb->current_local_tb = -1;

    return sym_tb;
}


void symb_tb_com_delete(symbol_tb_com *symb_tb){
    if(symb_tb){
        if(symb_tb->global) hash_tb_com_delete(symb_tb->global);
        for(int i=0; i<=symb_tb->current_local_tb; i++){
            if(symb_tb->local[i]) hash_tb_com_delete(symb_tb->local[i]);
        }
        free(symb_tb);
    }
}


int symb_tb_com_insert(symbol_tb_com *symb_tb, Symbol *symb){
    if(!symb_tb){
        perror("Symbol table NULL pointer when inserting");
        return -1;
    }
    if(!symb){
        perror("Symbol NULL pointer when inserting");
        return -1;
    }

    /* New domain */
    if(symb->symb_cat==FUNCTION && strcmp(CLOSURE_STR, symb->id)){
        if(symb_tb->current_local_tb>=0){
            /* Checking if key exists at last local table */
            if(hash_tb_com_isKey(symb_tb->local[symb_tb->current_local_tb], symb->id) == 1){
                /* The variable/domain already exists at last local table */
                return -1;
            }
        }
        else{
            if(hash_tb_com_isKey(symb_tb->global, symb->id) == 1){
                /* The variable/domain already exists at the global table */
                return -1;
            }
        }
        /* Checking if there is space left for another domain */
        if(symb_tb->current_local_tb == MAX_LOCAL_TB-1){
            /* Unable to create a new domain because the limit is already reached */
            return -1;
        }

        if(symb_tb->current_local_tb >= 0){
            /* Inserting in previous local table if any */
            hash_tb_com_insert(symb_tb->local[symb_tb->current_local_tb], symb);
        }
        else{
            /* Inserting in global table */
            hash_tb_com_insert(symb_tb->global, symb);
        }

        symb_tb->current_local_tb++; /* New domain index */
        /* Creating new local table */
        symb_tb->local[symb_tb->current_local_tb] = hash_tb_com_create(HASH_TB_SIZE, HASH_TB_CHAIN_SIZE, HT_DYN_RESZ_BOOL);
        /* Inserting in the new local table */
        hash_tb_com_insert(symb_tb->local[symb_tb->current_local_tb], symb);
    }

    /* Closing domain */
    else if(symb->symb_cat==FUNCTION && !strcmp(CLOSURE_STR, symb->id)){
        /* Deleting current local table */
        if(symb_tb->current_local_tb >= 0){
            if(symb_tb->local[symb_tb->current_local_tb]){
                hash_tb_com_delete(symb_tb->local[symb_tb->current_local_tb]);
            }
            symb_tb->local[symb_tb->current_local_tb] = NULL;
            symb_tb->current_local_tb--;
        }
    }

    /* New element */
    else{
        /* If any local table is initialized */
        if(symb_tb->current_local_tb>=0){
            if(hash_tb_com_isKey(symb_tb->local[symb_tb->current_local_tb], symb->id) == 1){
                /* The element already exists at the current local table */
                return -1;
            }
            /* Inserting in current local table */
            hash_tb_com_insert(symb_tb->local[symb_tb->current_local_tb], symb);
        }
        /* ZERO local table initialized ==>  Trying to insert in global table */
        else{
            if(hash_tb_com_isKey(symb_tb->global, symb->id) == 1){
                /* The element already exists at the global table */
                return -1;
            }
            /* Inserting in global table */
            hash_tb_com_insert(symb_tb->global, symb);
        }
    }
    return 0;
}


int symb_tb_com_isKey(symbol_tb_com *symb_tb, char *key){
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
            if(hash_tb_com_isKey(symb_tb->local[i], key) == 1) return 1;
        }
    }
    /* If local search fails, we must search in global table */
    if(hash_tb_com_isKey(symb_tb->global, key) == 1) return 1;
    /* Not found (or memory error at hash_tb_com_isKey funct's) */
    return 0;
}


Symbol * symb_tb_com_search(symbol_tb_com *symb_tb, char *key, int *is_local){
    if(!symb_tb){
        perror("Symbol table NULL pointer when searching");
        return NULL;
    }
    if(!key){
        perror("Key NULL pointer when searching");
        return NULL;
    }

    /* If any local table is not NULL we are in a local domain */
    /* We must search first at local domains */
    if(symb_tb->current_local_tb>=0){
        for(int i=symb_tb->current_local_tb; i>=0; i--){
            /* If the key is found in any local domain, it's no needed to keep searching */
            *is_local = 1;
            return hash_tb_com_get(symb_tb->local[i], key);
        }
    }
    /* If local search fails, we must search in global table */
    *is_local = 0
    return hash_tb_com_get(symb_tb->global, key);
}
