%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "alfa.h"

extern int yylex(void);
extern FILE* yyout;
extern size_t nlines;

void yyerror(const char * s);

int tipo_actual;
int clase_actual;
int pos_variable_local;
int num_variables_locales;
int num_parametros;
int pos_parametro;

int vector_size = 0;
int label = 1;
int active_func = 0; /* Flag that indicates if a function declaration is being processed */
int func_ret = 0; /* Flag that indicates if the current function has return statement */
int func_body = 0;
int main_declaration = 0;
int param_list=0;

symbol_tb_com *symb_tb=NULL;

%}

%union
{
    attr_type attributes;
}

/* Palabras reservadas */

%token TOK_MAIN
%token TOK_INT
%token TOK_BOOLEAN
%token TOK_ARRAY
%token TOK_FUNCTION
%token TOK_IF
%token TOK_ELSE
%token TOK_WHILE
%token TOK_SCANF
%token TOK_PRINTF
%token TOK_RETURN
%token TOK_PUNTOYCOMA
%token TOK_COMA
%token TOK_PARENTESISIZQUIERDO
%token TOK_PARENTESISDERECHO
%token TOK_CORCHETEIZQUIERDO
%token TOK_CORCHETEDERECHO
%token TOK_LLAVEIZQUIERDA
%token TOK_LLAVEDERECHA
%token TOK_ASIGNACION
%token TOK_MAS
%token TOK_MENOS
%token TOK_DIVISION
%token TOK_ASTERISCO
%token TOK_MODULO
%token TOK_MODULOVEC
%token TOK_SUMAVEC
%token TOK_ASTERISCOVEC
%token TOK_AND
%token TOK_OR
%token TOK_NOT
%token TOK_IGUAL
%token TOK_DISTINTO
%token TOK_MENORIGUAL
%token TOK_MAYORIGUAL
%token TOK_MENOR
%token TOK_MAYOR
%token TOK_TRUE
%token TOK_FALSE
%token TOK_ACENTO

%token <attributes> TOK_CONSTANTE_ENTERA
%token <attributes> TOK_IDENTIFICADOR

%type <attributes> condicional
%type <attributes> comparacion
%type <attributes> elemento_vector
%type <attributes> exp
%type <attributes> constante
%type <attributes> constante_entera
%type <attributes> constante_logica
%type <attributes> identificador
%type <attributes> if_exp
%type <attributes> if_exp_sentencias
%type <attributes> while
%type <attributes> while_exp
%type <attributes> bucle
%type <attributes> clase_vector
%type <attributes> funcion
%type <attributes> fn_declaration
%type <attributes> fn_name
%type <attributes> exp_fn
%type <attributes> lista_expresiones
%type <attributes> resto_lista_expresiones
%type <attributes> vec_id

%left TOK_IGUAL TOK_MENORIGUAL TOK_MENOR TOK_MAYORIGUAL TOK_MAYOR TOK_DISTINTO
%left TOK_AND TOK_OR
%left TOK_MAS TOK_MENOS
%left TOK_ASTERISCO TOK_DIVISION TOK_MODULO TOK_MODULOVEC
%right TOK_NOT

%%

programa:
    init_sbtb TOK_MAIN TOK_LLAVEIZQUIERDA declaraciones init_assembly funciones init_main sentencias TOK_LLAVEDERECHA {
        escribir_fin(yyout);
        symb_tb_com_delete(symb_tb);
        fprintf(yyout, ";R1:\t<programa> ::= main { <declaraciones> <funciones> <sentencias> }\n");
    }
;

init_sbtb: %empty {
    symb_tb = symb_tb_com_create();
    main_declaration = 1;
}

init_assembly: %empty {
    escribir_subseccion_data(yyout);
    escribir_cabecera_bss(yyout);
    Symbol ** list;
    Symbol *symb;
    int sz;
    list = symb_tb_com_get_list(symb_tb, &sz ,GLOBAL);
    for (size_t i = 0; i < sz; i++) {
        symb = list[i];
        int len = symb->len;
        if (symb->symb_type == INTEGER) {
            declarar_variable(yyout, symb->id, ENTERO, len);
        } else {
            declarar_variable(yyout, symb->id, BOOLEANO, len);
        }
        _symbol_delete(symb);
    }
    free(list);
    escribir_segmento_codigo(yyout);
}

init_main: %empty {
    main_declaration = 0;
    escribir_inicio_main(yyout);
}

declaraciones:
    declaracion { fprintf(yyout, ";R2:\t<declaraciones> ::= <declaracion>\n"); }
|   declaracion declaraciones { fprintf(yyout, ";R3:\t<declaraciones> ::= <declaracion> <declaraciones>\n"); }
;

declaracion:
    clase identificadores TOK_PUNTOYCOMA {
        fprintf(yyout, ";R4:\t<declaracion> ::= <clase> <identificadores> ;\n");
    }
;

clase:
    clase_escalar {
        clase_actual = SCALAR;
        fprintf(yyout, ";R5:\t<clase> ::= <clase_escalar>\n");
     }
|   clase_vector {
        clase_actual = VECTOR;
        fprintf(yyout, ";R7:\t<clase> ::= <clase_vector>\n");
    }
;

clase_escalar:
    tipo { fprintf(yyout, ";R9:\t<clase_escalar> ::= <tipo>\n"); }
;

tipo:
    TOK_INT {
        tipo_actual = INTEGER;
        fprintf(yyout, ";R10:\t<tipo> ::= int\n");
    }
|   TOK_BOOLEAN {
        tipo_actual = BOOLEAN;
        fprintf(yyout, ";R11:\t<tipo> ::= boolean\n");
    }
;

clase_vector:
    TOK_ARRAY tipo TOK_CORCHETEIZQUIERDO constante_entera TOK_CORCHETEDERECHO {
        $$.int_value = $4.int_value;
        vector_size = $4.int_value;
        fprintf(yyout, ";R15:\t<clase_vector> ::= array <tipo> [ <constante_entera> ]\n");
    }
;

identificadores:
    identificador { fprintf(yyout, ";R18:\t<identificadores> ::= <identificador>\n"); }
|   identificador TOK_COMA identificadores { fprintf(yyout, ";R19:\t<identificadores> ::= <identificador> , <identificadores>\n"); }
;

funciones:
    funcion funciones { fprintf(yyout, ";R20:\t<funciones> ::= <funcion> <funciones>\n"); }
|   %empty { fprintf(yyout, ";R21:\t<funciones> ::=\n"); }
;

funcion: fn_declaration sentencias TOK_LLAVEDERECHA {
    if(func_ret == 0) {
        printf("****Error semantico en lin %ld: Funcion %s sin sentencia de retorno.\n", nlines, $1.lexeme);
        return -1;
    }
    num_parametros = 0;
    num_variables_locales = 0;
    func_ret = 0;
    func_body = 0;
    // Close domain
    Symbol *symb=NULL;
    symb = (Symbol *) calloc(1, sizeof(Symbol));
    symb->id = (char *) calloc(strlen("cierre")+1, sizeof(char));

    strcpy(symb->id, "cierre");
    symb->symb_cat = FUNCTION;
    symb_tb_com_insert(symb_tb, symb);
    _symbol_delete(symb);

    fprintf(yyout, ";R22:\t<funcion> ::= function <tipo> <identificador> ( <parametros_funcion> ) { <declaraciones_funcion> <sentencias> }\n");
};

fn_declaration : fn_name TOK_PARENTESISIZQUIERDO parametros_funcion TOK_PARENTESISDERECHO TOK_LLAVEIZQUIERDA declaraciones_funcion {
    int is_local;
    Symbol *symb = symb_tb_com_search(symb_tb, $1.lexeme, &is_local);
    symb->num_param = num_parametros;
    symb->num_local_var = num_variables_locales;
    symb_tb_com_update(symb_tb, symb->id, symb);
    symb->symb_type = $1.type;
    strcpy($$.lexeme, $1.lexeme);
    $$.type = $1.type;
    declararFuncion(yyout, $1.lexeme, num_variables_locales);
    active_func = 0;
    _symbol_delete(symb);
}

fn_name : TOK_FUNCTION tipo TOK_IDENTIFICADOR {
    active_func = 1;
    func_body = 1;
    func_ret = 0;
    int is_local;
    Symbol *symb = symb_tb_com_search(symb_tb, $3.lexeme, &is_local);
    if(symb){
        _symbol_delete(symb);
        printf("****Error semantico en lin %ld: Declaracion duplicada.\n", nlines);
        return -1;
    }
    // Creating new symbol
    Symbol *insert_symb=NULL;
    insert_symb = (Symbol *) calloc(1, sizeof(Symbol));
    insert_symb->id = (char *) calloc(strlen($3.lexeme)+1, sizeof(char));
    strcpy(insert_symb->id, $3.lexeme);
    insert_symb->symb_cat = FUNCTION;
    insert_symb->symb_type = tipo_actual;
    $$.type = tipo_actual;
    strcpy($$.lexeme, $3.lexeme);
    symb_tb_com_insert(symb_tb, insert_symb);
    _symbol_delete(insert_symb);

    pos_variable_local = 0;
    num_variables_locales = 0;
    pos_parametro = 0;
    num_parametros = 0;
}

parametros_funcion:
    parametro_funcion resto_parametros_funcion { fprintf(yyout, ";R23:\t<parametros_funcion> ::= <parametro_funcion> <resto_parametros_funcion>\n"); }
|   %empty { fprintf(yyout, ";R24:\t<parametros_funcion> ::=\n"); }
;

resto_parametros_funcion:
    TOK_PUNTOYCOMA parametro_funcion resto_parametros_funcion { fprintf(yyout, ";R25:\t<resto_parametros_funcion> ::= ; <parametro_funcion> <resto_parametros_funcion>\n"); }
|   %empty { fprintf(yyout, ";R26:\t<resto_parametros_funcion> ::=\n"); }
;

parametro_funcion:
    tipo idpf {
        num_parametros++;
        pos_parametro++;
        fprintf(yyout, ";R27:\t<parametro_funcion> ::= <tipo> <identificador>\n");
    }
;

idpf: TOK_IDENTIFICADOR {
    int is_local;
    Symbol *symb = symb_tb_com_search(symb_tb, $1.lexeme, &is_local);
    if(symb && is_local){
        printf("****Error semantico en lin %ld: Declaracion duplicada.\n", nlines);
        return -1;
    }
    _symbol_delete(symb);

    /* Creating symbol */
    Symbol *insert_symb=NULL;
    insert_symb = (Symbol *) calloc(1, sizeof(Symbol));
    insert_symb->id = (char *) calloc(strlen($1.lexeme)+1, sizeof(char));

    strcpy(insert_symb->id, $1.lexeme);
    insert_symb->symb_cat = PARAMETER;
    insert_symb->symb_type = SCALAR;
    insert_symb->symb_type = tipo_actual;
    insert_symb->pos = pos_parametro;
    symb_tb_com_insert(symb_tb, insert_symb);
    _symbol_delete(insert_symb);

}

declaraciones_funcion:
    declaraciones { fprintf(yyout, ";R28:\t<declaraciones_funcion> ::= <declaraciones>\n"); }
|   %empty { fprintf(yyout, ";R28:\t<declaraciones_funcion> ::=\n"); }
;

sentencias:
    sentencia { fprintf(yyout, ";R30:\t<sentencias> ::= <sentencia>\n"); }
|   sentencia sentencias { fprintf(yyout, ";R31:\t<sentencias> ::= <sentencia> <sentencias>\n"); }
;

sentencia:
    sentencia_simple TOK_PUNTOYCOMA {
        param_list = 0;
        fprintf(yyout, ";R32:\t<sentencia> ::= <sentencia_simple> ;\n"); }
|   bloque { fprintf(yyout, ";R33:\t<sentencia> ::= <bloque>\n"); }
;

sentencia_simple:
    asignacion { fprintf(yyout, ";R34:\t<sentencia_simple> ::= <asignacion>\n"); }
|   lectura { fprintf(yyout, ";R35:\t<sentencia_simple> ::= <lectura>\n"); }
|   escritura { fprintf(yyout, ";R36:\t<sentencia_simple> ::= <escritura>\n"); }
|   retorno_funcion {
        if (!func_body) {
            printf("****Error semantico en lin %lu: Sentencia de retorno fuera del cuerpo de una función.\n", nlines);
            return -1;
        }
        fprintf(yyout, ";R38:\t<sentencia_simple> ::= <retorno_funcion>\n");
    }
|   vec_id TOK_MODULOVEC exp {
        if ($1.type == BOOLEAN || $3.type == BOOLEAN){
            printf("****Error semantico en lin %lu: Operacion aritmetica con operandos boolean.\n", nlines);
            return -1;
        }

        fprintf(yyout, "pop dword ecx\n");

        char op[255];
        for(int i=0; i<$1.vec_size; i++){
            sprintf(op, "%d", i);
            // escribir el indice del vector
            escribir_operando(yyout, op, 0);
            // escribir el elemento del vector indexado por el indice anterior
            // (el indice sale de la pila y entra el vector[i])
            escribir_elemento_vector(yyout, $1.lexeme, $1.vec_size, 0);
            // escribir el operando por el que se va a hacer modulo
            fprintf(yyout, "push dword ecx\n");
            // el primero es un elemento vector (variable) y el otro un IMM
            modulo(yyout, 1, 0);
            // AHORA ASIGNAMOS
            // escribir el indice del vector
            escribir_operando(yyout, op, 0);
            // escribir el elemento del vector indexado por el indice anterior
            // (el indice sale de la pila y entra el vector[i])
            escribir_elemento_vector(yyout, $1.lexeme, $1.vec_size, 0);
            // 0 porque el resultado del modulo ya es un IMM
            asignarDestinoEnPila(yyout, 0);

        }

        fprintf(yyout, ";R74:\t<sentencia_simple> ::= <vec_id> %% <exp>\n");
    }
|   vec_id TOK_SUMAVEC exp {
        if ($1.type != $3.type){
            printf("****Error semantico en lin %lu: Opereación vectorial con tipos diferentes.\n", nlines);
            return -1;
        }

        fprintf(yyout, "pop dword ecx\n");

        char op[255];
        for(int i=0; i<$1.vec_size; i++){
            sprintf(op, "%d", i);
            // escribir el indice del vector
            escribir_operando(yyout, op, 0);
            // escribir el elemento del vector indexado por el indice anterior
            // (el indice sale de la pila y entra el vector[i])
            escribir_elemento_vector(yyout, $1.lexeme, $1.vec_size, 0);
            // escribir el operando por el que se va a hacer modulo
            fprintf(yyout, "push dword ecx\n");
            // el primero es un elemento vector (variable) y el otro un IMM
            if ($1.type == BOOLEAN) {
                o(yyout, 1, 0);
            } else {
                sumar(yyout, 1, 0);
            }
            // AHORA ASIGNAMOS
            // escribir el indice del vector
            escribir_operando(yyout, op, 0);
            // escribir el elemento del vector indexado por el indice anterior
            // (el indice sale de la pila y entra el vector[i])
            escribir_elemento_vector(yyout, $1.lexeme, $1.vec_size, 0);
            // 0 porque el resultado del modulo ya es un IMM
            asignarDestinoEnPila(yyout, 0);

        }
        fprintf(yyout, ";R74:\t<sentencia_simple> ::= <vec_id> .+ <exp>\n");
    }
|   vec_id TOK_ASTERISCOVEC exp {
        if ($1.type != $3.type){
            printf("****Error semantico en lin %lu: Opereación vectorial con tipos diferentes.\n", nlines);
            return -1;
        }

        fprintf(yyout, "pop dword ecx\n");

        char op[255];
        for(int i=0; i<$1.vec_size; i++){
            sprintf(op, "%d", i);
            // escribir el indice del vector
            escribir_operando(yyout, op, 0);
            // escribir el elemento del vector indexado por el indice anterior
            // (el indice sale de la pila y entra el vector[i])
            escribir_elemento_vector(yyout, $1.lexeme, $1.vec_size, 0);
            // escribir el operando por el que se va a hacer modulo
            fprintf(yyout, "push dword ecx\n");
            // el primero es un elemento vector (variable) y el otro un IMM
            if ($1.type == BOOLEAN) {
                y(yyout, 1, 0);
            } else {
                multiplicar(yyout, 1, 0);
            }
            // AHORA ASIGNAMOS
            // escribir el indice del vector
            escribir_operando(yyout, op, 0);
            // escribir el elemento del vector indexado por el indice anterior
            // (el indice sale de la pila y entra el vector[i])
            escribir_elemento_vector(yyout, $1.lexeme, $1.vec_size, 0);
            // 0 porque el resultado del modulo ya es un IMM
            asignarDestinoEnPila(yyout, 0);

        }
        fprintf(yyout, ";R74:\t<vec_id> ::= <identificador> .* <exp>\n");
    }
;

bloque:
    condicional { fprintf(yyout, ";R40:\t<bloque> ::= <condicional>\n"); }
|   bucle { fprintf(yyout, ";R41:\t<bloque> ::= <bucle>\n"); }
;

asignacion:
    TOK_IDENTIFICADOR TOK_ASIGNACION exp {
        fprintf(yyout, ";R43:\t<asignacion> ::= <identificador> = <exp>\n");
        int is_local = -1;
        Symbol *symb=NULL;
        symb = symb_tb_com_search(symb_tb, $1.lexeme, &is_local);
        if(!symb){
            printf("****Error semantico en lin %lu: Acceso a variable no declarada (%s).\n", nlines, $1.lexeme);
            return -1;
        }
        if(symb->symb_cat == FUNCTION || symb->var_cat == VECTOR || symb->symb_type != $3.type){
            printf("****Error semantico en lin %lu: Asignacion incompatible.\n", nlines);
            _symbol_delete(symb);
            return -1;
        }

        if(is_local){ /* Local variable */
            if(symb->symb_cat == PARAMETER){
                escribirParametro(yyout, symb->pos, num_parametros);
                asignarDestinoEnPila(yyout, $3.is_address);
            } else {
                escribirVariableLocal(yyout, symb->pos);
                asignarDestinoEnPila(yyout, $3.is_address);
            }
        } else {
            asignar(yyout, symb->id, $3.is_address);
        }

        _symbol_delete(symb);
    }
|   elemento_vector TOK_ASIGNACION exp {
        fprintf(yyout, ";R44:\t<asignacion> ::= <elemento_vector> = <exp>\n");
        if($1.type != $3.type) {
            printf("****Error semantico en lin %ld: Asignacion incompatible.\n", nlines);
            return -1;
        }
        int is_local;
        Symbol *symb = symb_tb_com_search(symb_tb, $1.lexeme, &is_local);
        if(!symb){
            printf("****Error semantico en lin %lu: Acceso a variable no declarada (%s).\n", nlines, $1.lexeme);
            return -1;
        }
        escribir_operando(yyout, $1.vector_index, $1.vector_index_is_address);
        escribir_elemento_vector(yyout, $1.lexeme, symb->len, $3.is_address);
        asignarDestinoEnPila(yyout, $3.is_address);
        _symbol_delete(symb);
    }
;

elemento_vector:
    TOK_IDENTIFICADOR TOK_CORCHETEIZQUIERDO exp TOK_CORCHETEDERECHO {
        if($3.type != INTEGER){
            printf("****Error semantico en lin %ld: El indice en una operacion de indexacion tiene que ser de tipo entero.\n", nlines);
            return -1;
        }

        int is_local;
        Symbol *symb = symb_tb_com_search(symb_tb, $1.lexeme, &is_local);
        if(!symb){
            printf("****Error semantico en lin %ld: Acceso a variable no declarada (%s).\n", nlines, $1.lexeme);
            return -1;
        }
        if(symb->symb_cat == FUNCTION){
            printf("****Error semantico en lin %ld: Intento de indexacion de una variable que no es de tipo vector.\n", nlines);
            _symbol_delete(symb);
            return -1;
        }
        if(symb->var_cat != VECTOR){
            printf("****Error semantico en lin %ld: Intento de indexacion de una variable que no es de tipo vector.\n", nlines);
            _symbol_delete(symb);
            return -1;
        }

        $$.type = symb->symb_type;
        $$.is_address = 1;
        $$.vector_index_is_address = $3.is_address;
        $$.int_value = $3.int_value;
        strcpy($$.vector_index, $3.lexeme);
        escribir_elemento_vector(yyout, $1.lexeme, symb->len, $3.is_address);
        fprintf(yyout, ";R48:\t<elemento_vector> ::= <identificador>\n");
        _symbol_delete(symb);
    }
;

condicional:
    if_exp_sentencias {
        ifthenelse_fin(yyout, $1.label);
        fprintf(yyout, ";R50:\t<condicional> ::= if ( <exp> ) { <sentencias> }\n");
    }
|   if_exp_sentencias TOK_ELSE TOK_LLAVEIZQUIERDA sentencias TOK_LLAVEDERECHA {
        ifthenelse_fin(yyout, $1.label);
        fprintf(yyout, ";R50:\t<condicional> ::= if ( <exp> ) { <sentencias> } else { <sentencias> }\n");
    }
;

if_exp_sentencias:
    if_exp sentencias TOK_LLAVEDERECHA {
        $$.label = $1.label;
        ifthenelse_fin_then(yyout, $$.label);
    }
;

if_exp:
    TOK_IF TOK_PARENTESISIZQUIERDO exp TOK_PARENTESISDERECHO TOK_LLAVEIZQUIERDA {
        if ($3.type != BOOLEAN) {
            printf("****Error semantico en lin %lu: Condicional con condicion de tipo int.\n", nlines);
            return -1;
        }
        $$.label = label++;
        ifthenelse_inicio(yyout, $3.is_address, $$.label);
    }
;

bucle:
    while_exp sentencias TOK_LLAVEDERECHA {
        while_fin(yyout, $1.label);
    }
;

while:
    TOK_WHILE TOK_PARENTESISIZQUIERDO {
        $$.label = label++;
        while_inicio(yyout, $$.label);
    }
;

while_exp:
    while exp TOK_PARENTESISDERECHO TOK_LLAVEIZQUIERDA {
        if($2.type != BOOLEAN){
            printf("****Error semantico en lin %lu: Bucle con condicion de tipo int.\n", nlines);
            return -1;
        }
        while_exp_pila(yyout, $2.is_address, $$.label);
    }
;

lectura:
    TOK_SCANF identificador {
        fprintf(yyout, ";R54:\t<lectura> ::= scanf <identificador>\n");
        int is_local = -1;
        Symbol *symb=NULL;
        symb = symb_tb_com_search(symb_tb, $2.lexeme, &is_local);
        if(!symb){
            printf("****Error semantico en lin %ld: Acceso a variable no declarada (%s).\n", nlines, $2.lexeme);
            return -1;
        }
        if(symb->symb_cat == FUNCTION || symb->var_cat == VECTOR){
            printf("****Error semantico en lin %ld: Asignacion incompatible.\n", nlines);
            _symbol_delete(symb);
            return -1;
        }

        leer(yyout, symb->id, symb->symb_type);
        _symbol_delete(symb);
    }
;

escritura:
    TOK_PRINTF exp {
        escribir(yyout, $2.is_address, $2.type);
        fprintf(yyout, ";R56:\t<escritura> ::= printf <exp>\n");
    }
;

retorno_funcion:
    TOK_RETURN exp {
        func_ret = 1;
        retornarFuncion(yyout, $2.is_address);
        fprintf(yyout, ";R61:\t<retorno_funcion> ::= return <exp>\n");
    }
;

exp:
    exp TOK_MAS exp {
        if ($1.type != $3.type ){
            printf("****Error semantico en lin %lu: Operación con operandos de distinto tipo.\n", nlines);
            return -1;
        }
        $$.type = $1.type;
        $$.is_address = 0;
        if ($1.type == BOOLEAN){
            $$.int_value = $1.int_value || $3.int_value;
            o(yyout, $1.is_address, $3.is_address);
        } else {
            $$.int_value = $1.int_value + $3.int_value;
            sumar(yyout, $1.is_address, $3.is_address);
        }
        fprintf(yyout, ";R72:\t<exp> ::= <exp> + <exp>\n");
    }
|   exp TOK_MENOS exp {
        if ($1.type == BOOLEAN || $3.type == BOOLEAN){
            printf("****Error semantico en lin %lu: Operacion aritmetica con operandos boolean.\n", nlines);
            return -1;
        }
        $$.type = INTEGER;
        $$.is_address = 0;
        $$.int_value = $1.int_value - $3.int_value;
        restar(yyout, $1.is_address, $3.is_address);
        fprintf(yyout, ";R73:\t<exp> ::= <exp> - <exp>\n");
    }
|   exp TOK_DIVISION exp {
        if ($1.type == BOOLEAN || $3.type == BOOLEAN){
            printf("****Error semantico en lin %lu: Operacion aritmetica con operandos boolean.\n", nlines);
            return -1;
        }
        $$.type = INTEGER;
        $$.is_address = 0;
        if (!$3.int_value) {
            $$.int_value = -1;
        } else {
            $$.int_value = $1.int_value / $3.int_value;
        }
        dividir(yyout, $1.is_address, $3.is_address);
        fprintf(yyout, ";R74:\t<exp> ::= <exp> / <exp>\n");
    }
|   exp TOK_MODULO exp {
            if ($1.type == BOOLEAN || $3.type == BOOLEAN){
                printf("****Error semantico en lin %lu: Operacion aritmetica con operandos boolean.\n", nlines);
                return -1;
            }
            $$.type = INTEGER;
            $$.is_address = 0;
            if (!$3.int_value) {
                $$.int_value = -1;
            } else {
                $$.int_value = $1.int_value % $3.int_value;
            }
            modulo(yyout, $1.is_address, $3.is_address);
            fprintf(yyout, ";R74:\t<exp> ::= <exp> %% <exp>\n");
        }
|   exp TOK_ASTERISCO exp {
    if ($1.type != $3.type ){
        printf("****Error semantico en lin %lu: Operación con operandos de distinto tipo.\n", nlines);
        return -1;
    }
    $$.type = $1.type;
    $$.is_address = 0;
    if ($1.type == BOOLEAN){
        $$.int_value = $1.int_value && $3.int_value;
        y(yyout, $1.is_address, $3.is_address);
    } else {
        $$.int_value = $1.int_value * $3.int_value;
        multiplicar(yyout, $1.is_address, $3.is_address);
    }
    fprintf(yyout, ";R75:\t<exp> ::= <exp> * <exp>\n");
    }
|   TOK_MENOS exp {
        if ($2.type == BOOLEAN){
            printf("****Error semantico en lin %lu: Operacion aritmetica con operandos boolean.\n", nlines);
            return -1;
        }
        $$.type = INTEGER;
        $$.is_address = 0;
        $$.int_value = -1 * $2.int_value;
        cambiar_signo(yyout, $2.is_address);
        fprintf(yyout, ";R76:\t<exp> ::= -<exp>\n");
    }
|   exp TOK_AND exp {
        if ($1.type == INTEGER || $3.type == INTEGER){
            printf("****Error semantico en lin %lu: Operacion logica con operandos int.\n", nlines);
            return -1;
        }
        $$.type = BOOLEAN;
        $$.is_address = 0;
        $$.int_value = $1.int_value && $3.int_value;
        y(yyout, $1.is_address, $3.is_address);
        fprintf(yyout, ";R77:\t<exp> ::= <exp> && <exp>\n");
    }
|   exp TOK_OR exp {
        if ($1.type == INTEGER || $3.type == INTEGER){
            printf("****Error semantico en lin %lu: Operacion logica con operandos int.\n", nlines);
            return -1;
        }
        $$.type = BOOLEAN;
        $$.is_address = 0;
        $$.int_value = $1.int_value || $3.int_value;
        o(yyout, $1.is_address, $3.is_address);
        fprintf(yyout, ";R78:\t<exp> ::= <exp> || <exp>\n");
    }
|   TOK_NOT exp {
        if ($2.type == INTEGER){
            printf("****Error semantico en lin %lu: Operacion logica con operandos int.\n", nlines);
            return -1;
        }
        $$.type = BOOLEAN;
        $$.is_address = 0;
        $$.int_value = !$2.int_value;
        no(yyout, $2.is_address, label++);
        fprintf(yyout, ";R79:\t<exp> ::= !<exp>\n");
    }
|   TOK_IDENTIFICADOR {
        fprintf(yyout, ";R80:\t<exp> ::= <identificador>\n");
        int is_local = -1;
        Symbol *symb=NULL;
        symb = symb_tb_com_search(symb_tb, $1.lexeme, &is_local);
        if(!symb){
            printf("****Error semantico en lin %ld: Acceso a variable no declarada (%s).\n", nlines, $1.lexeme);
            return -1;
        }
        if(symb->symb_cat == FUNCTION || symb->var_cat == VECTOR){
            _symbol_delete(symb);
            printf("****Error semantico en lin %ld: Asignacion incompatible.\n", nlines);
            return -1;
        }

        $$.type = symb->symb_type;
        $$.is_address = 1;

        if(is_local){ /* Local variable */
            if(symb->symb_cat == PARAMETER){
                escribirParametro(yyout, symb->pos, num_parametros);
            } else {
                escribirVariableLocal(yyout, symb->pos);
            }
        } else {
            escribir_operando(yyout, symb->id, 1);
        }

        _symbol_delete(symb);
    }
|   TOK_ACENTO vec_id {
        char op[255];
        sprintf(op, "%d", $2.vec_size);
        escribir_operando(yyout, op, 0);
        fprintf(yyout, ";R800:\t<exp> ::= ^<vec_id>\n");
    }
|   constante {
        $$.type = $1.type;
        $$.is_address = 0;
        sprintf($$.lexeme, "%d", $1.int_value);
        escribir_operando(yyout, $$.lexeme, $1.is_address);
        fprintf(yyout, ";R81:\t<exp> ::= <constante>\n");
    }
|   TOK_PARENTESISIZQUIERDO exp TOK_PARENTESISDERECHO {
        $$.type = $2.type;
        $$.is_address = $2.is_address;
        $$.int_value = $2.int_value;
        fprintf(yyout, ";R82:\t<exp> ::= (<exp>)\n");
    }
|   TOK_PARENTESISIZQUIERDO comparacion TOK_PARENTESISDERECHO {
        $$.type = $2.type;
        $$.is_address = $2.is_address;
        $$.int_value = $2.int_value;
        fprintf(yyout, ";R83:\t<exp> ::= (<comparacion>)\n");
    }
|   elemento_vector { fprintf(yyout, ";R85:\t<exp> ::= <elemento_vector>\n"); }
|   identificador TOK_PARENTESISIZQUIERDO lista_expresiones TOK_PARENTESISDERECHO {
        int is_local = -1;
        Symbol *symb=NULL;
        symb = symb_tb_com_search(symb_tb, $1.lexeme, &is_local);
        if(!symb){
            printf("****Error semantico en lin %ld: Acceso a variable no declarada (%s).\n", nlines, $1.lexeme);
            return -1;
        }
        if (symb->num_param != $3.int_value){
            printf("****Error semantico en lin %ld: Numero incorrecto de parametros en llamada a funcion.\n", nlines);
            _symbol_delete(symb);
            return -1;
        }
        $$.type = symb->symb_type;
        llamarFuncion(yyout, $1.lexeme, symb->num_param);
        param_list++;
        if (param_list > 1) {
            printf("****Error semantico en lin %ld: No esta permitido el uso de llamadas a funciones como parametros de otras funciones.\n", nlines);
            _symbol_delete(symb);
            return -1;
        }
        fprintf(yyout, ";R88:\t<exp> ::= <identificador> ( <lista_expresiones> )\n");
        _symbol_delete(symb);
    }
;

exp_fn: exp {
    operandoEnPilaAArgumento(yyout, $1.is_address);
}

lista_expresiones:
    exp_fn resto_lista_expresiones {$$.int_value = $2.int_value+1; fprintf(yyout, ";R89:\t<lista_expresiones> ::= <exp> <resto_lista_expresiones>\n");}
|   %empty {fprintf(yyout, ";R90:\t<lista_expresiones> ::=\n"); }
;

resto_lista_expresiones:
    TOK_COMA exp_fn resto_lista_expresiones {$$.int_value = $3.int_value+1; fprintf(yyout, ";R91:\t<resto_lista_expresiones> ::= , <exp> <resto_lista_expresiones>\n"); }
|   %empty { $$.int_value = 0; fprintf(yyout, ";R92:\t<resto_lista_expresiones> ::=\n"); }
;

comparacion:
    exp TOK_IGUAL exp {
        if ($1.type == BOOLEAN || $3.type == BOOLEAN){
            printf("****Error semantico en lin %lu: Comparacion con operandos boolean\n.", nlines);
            return -1;
        }
        igual(yyout, $1.is_address, $3.is_address, label++);
        $$.type = BOOLEAN;
        $$.is_address = 0;
        $$.int_value = $1.int_value == $3.int_value;
        fprintf(yyout, ";R93:\t<comparacion> ::= <exp> == <exp>\n");
    }
|   exp TOK_DISTINTO exp {
        if ($1.type == BOOLEAN || $3.type == BOOLEAN){
            printf("****Error semantico en lin %lu: Comparacion con operandos boolean\n.", nlines);
            return -1;
        }
        distinto(yyout, $1.is_address, $3.is_address, label++);
        $$.type = BOOLEAN;
        $$.is_address = 0;
        $$.int_value = $1.int_value != $3.int_value;
        fprintf(yyout, ";R94:\t<comparacion> ::= <exp> != <exp>\n");
    }
|   exp TOK_MENORIGUAL exp {
        if ($1.type == BOOLEAN || $3.type == BOOLEAN){
            printf("****Error semantico en lin %lu: Comparacion con operandos boolean\n.", nlines);
            return -1;
        }
        menor_igual(yyout, $1.is_address, $3.is_address, label++);
        $$.type = BOOLEAN;
        $$.is_address = 0;
        $$.int_value = $1.int_value <= $3.int_value;
        fprintf(yyout, ";R95:\t<comparacion> ::= <exp> <= <exp>\n");
    }
|   exp TOK_MAYORIGUAL exp {
        if ($1.type == BOOLEAN || $3.type == BOOLEAN){
            printf("****Error semantico en lin %lu: Comparacion con operandos boolean\n.", nlines);
            return -1;
        }
        mayor_igual(yyout, $1.is_address, $3.is_address, label++);
        $$.type = BOOLEAN;
        $$.is_address = 0;
        $$.int_value = $1.int_value >= $3.int_value;
        fprintf(yyout, ";R96:\t<comparacion> ::= <exp> >= <exp>\n");
    }
|   exp TOK_MENOR exp {
        if ($1.type == BOOLEAN || $3.type == BOOLEAN){
            printf("****Error semantico en lin %lu: Comparacion con operandos boolean\n.", nlines);
            return -1;
        }
        menor(yyout, $1.is_address, $3.is_address, label++);
        $$.type = BOOLEAN;
        $$.is_address = 0;
        $$.int_value = $1.int_value < $3.int_value;
        fprintf(yyout, ";R97:\t<comparacion> ::= <exp> < <exp>\n");
    }
|   exp TOK_MAYOR exp {
        if ($1.type == BOOLEAN || $3.type == BOOLEAN){
            printf("****Error semantico en lin %lu: Comparacion con operandos boolean\n.", nlines);
            return -1;
        }
        mayor(yyout, $1.is_address, $3.is_address, label++);
        $$.type = BOOLEAN;
        $$.is_address = 0;
        $$.int_value = $1.int_value > $3.int_value;
        fprintf(yyout, ";R98:\t<comparacion> ::= <exp> > <exp>\n");
    }
;

constante:
    constante_entera {
        $$.type = $1.type;
        $$.is_address = $1.is_address;
        fprintf(yyout, ";R99:\t<constante> ::= <constante_entera>\n");
    }
|   constante_logica { fprintf(yyout, ";R100:\t<constante> ::= <constante_logica>\n"); }
;

constante_logica:
    TOK_TRUE {
        $$.type = BOOLEAN;
        $$.is_address = 0;
        $$.int_value = 1;
        fprintf(yyout, ";R102:\t<constante_logica> ::= true\n");
    }
|   TOK_FALSE {
        $$.type = BOOLEAN;
        $$.is_address = 0;
        $$.int_value = 0;
        fprintf(yyout, ";R103:\t<constante_logica> ::= false\n");
    }
;

constante_entera: TOK_CONSTANTE_ENTERA {
    $$.type = INTEGER;
    $$.is_address = 0;
    $$.int_value = $1.int_value;
    fprintf(yyout, ";R104:\t<constante_entera> ::= TOK_CONSTANTE_ENTERA\n");
}
;

identificador: TOK_IDENTIFICADOR {
    int is_local = -1;
    Symbol *symb=NULL;
    symb = symb_tb_com_search(symb_tb, $1.lexeme, &is_local);

    if (main_declaration){
        if(symb && !(symb->symb_cat == FUNCTION)) {
            printf("****Error semantico en lin %lu: Declaracion duplicada.\n", nlines);
            _symbol_delete(symb);
            return -1;
        }

        if (clase_actual == VECTOR) {
            if(vector_size <= 0){
                printf("****Error semantico en lin %ld: El tamanyo del vector %s excede los limites permitidos (1,64).\n", nlines, $1.lexeme);
                _symbol_delete(symb);
                return -1;
            }
            if (vector_size > MAX_VECTOR_SIZE){
                printf("****Error semantico en lin %ld: El tamanyo del vector %s excede los limites permitidos (1,64).\n", nlines, $1.lexeme);
                _symbol_delete(symb);
                return -1;
            }
        }
    }

    _symbol_delete(symb);
    symb = (Symbol *) calloc(1, sizeof(Symbol));
    symb->id = (char *) calloc(strlen($1.lexeme)+1, sizeof(char));

    strcpy(symb->id, $1.lexeme);
    symb->symb_cat = VARIABLE;
    symb->symb_type = tipo_actual;
    symb->var_cat = clase_actual;

    if(clase_actual == VECTOR) {

        if (func_body) {
            printf("****Error semantico en lin %ld: Variable local de tipo no escalar.\n", nlines);
            return -1;
        }
        symb->len = vector_size;
        vector_size = 0;
    } else {
        symb->len = 1;
    }

    if (active_func){
        symb->pos = num_variables_locales++;
    }
    symb_tb_com_insert(symb_tb, symb);
    _symbol_delete(symb);

    fprintf(yyout, ";R108:\t<identificador> ::= TOK_IDENTIFICADOR\n");
}
;

vec_id: TOK_IDENTIFICADOR {
    int is_local = -1;
    Symbol *symb=NULL;
    symb = symb_tb_com_search(symb_tb, $1.lexeme, &is_local);
    if(!symb){
        printf("****Error semantico en lin %ld: Acceso a variable no declarada (%s).\n", nlines, $1.lexeme);
        return -1;
    }
    if(symb->var_cat != VECTOR){
        _symbol_delete(symb);
        printf("****Error semantico en lin %lu: Operacion modulo vector a un escalar\n", nlines);
        return -1;
    }
    $$.type = symb->symb_type;
    $$.is_address = 1;
    if (symb->var_cat == VECTOR)
        $$.vec_size = symb->len;
    strcpy($$.lexeme, symb->id);
}

%%

void yyerror(const char * s) {
    extern int is_morpho;
    extern int yyleng;
    extern size_t ncols;
    extern size_t nlines;
    if(!is_morpho) {
        printf("****Error sintactico en [lin %lu, col %lu]\n", nlines, ncols-yyleng);
    }
}
