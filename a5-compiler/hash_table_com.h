#ifndef HASH_TABLE_COM_H
#define HASH_TABLE_COM_H

#include <stdint.h>

/* Symbol category: variable, function parameter or function */
typedef enum {VARIABLE, PARAMETER, FUNCTION} Symbol_category;

/* Symbol type: we are  only working with integers and booleans */
typedef enum {INTEGER, BOOLEAN} Symbol_type;

/* Variable category: it can be atomic (scalar) or an array (vector) */
typedef enum {SCALAR, VECTOR} Variable_category;

/* Symbol information */
typedef struct {
    char *id;                       /* Symbol identifier */
    Symbol_category symb_cat;       /* Symbol category */
    Symbol_type symb_type;          /* Symbol type */
    Variable_category var_cat;      /* Variable category */
    int value;                      /* value if it is scalar */
    int len;                        /* length if it is vector */
    int num_param;                  /* Number of parameters if function */
    int pos;                        /* Parameter position if function parameter OR declaration position if local variable of a function */
    int num_local_var;              /* Number of local variables if function */
} Symbol;

typedef struct _hash_tb_com hash_tb_com;


hash_tb_com * hash_tb_com_create(size_t ht_sz, size_t chain_sz, size_t dyn_resz);

void hash_tb_com_delete(hash_tb_com *ht);

int hash_tb_com_insert(hash_tb_com *ht, Symbol *symb);

int hash_tb_com_isKey(hash_tb_com *ht, char *key);

Symbol * hash_tb_com_get(hash_tb_com *ht, char *key);

int hash_tb_com_print(hash_tb_com *ht);

int hash_tb_com_n_elem (hash_tb_com *ht);

void hash_tb_com_dump(hash_tb_com *ht, Symbol **list);

void _symbol_delete(Symbol *symb);

#endif /* HASH_TABLE_COM_H */
