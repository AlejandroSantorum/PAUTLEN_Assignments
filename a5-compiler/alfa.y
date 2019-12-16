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
int vector_size = 0;

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

%left TOK_IGUAL TOK_MENORIGUAL TOK_MENOR TOK_MAYORIGUAL TOK_MAYOR TOK_DISTINTO
%left TOK_AND TOK_OR
%left TOK_MAS TOK_MENOS
%left TOK_ASTERISCO TOK_DIVISION
%right TOK_NOT

%%

programa:
    init_sbtb TOK_MAIN TOK_LLAVEIZQUIERDA declaraciones init_assembly funciones init_main sentencias TOK_LLAVEDERECHA {
        escribir_fin(yyout);
        fprintf(stderr, ";R1:\t<programa> ::= main { <declaraciones> <funciones> <sentencias> }\n");
    }
;

init_sbtb: %empty {
    fprintf(stderr, ";DEBUG: Creating symbol table\n");
    symb_tb = symb_tb_com_create();
    /* TODO : Error check ? */
}

init_assembly: %empty {
    fprintf(stderr, ";DEBUG: Writing first chunk of assembly code\n");
    escribir_subseccion_data(yyout);
    escribir_cabecera_bss(yyout);
    Symbol ** list;
    Symbol *symb;
    int sz;
    list = symb_tb_com_get_list(symb_tb, &sz ,GLOBAL);
    for (size_t i = 0; i < sz; i++) {
        symb = list[i];
        if (symb->symb_type == INTEGER) {
            declarar_variable(yyout, symb->id, ENTERO, 1);
        } else {
            declarar_variable(yyout, symb->id, BOOLEANO, 1);
        }
        _symbol_delete(symb);
    }
    free(list);
    escribir_segmento_codigo(yyout);
    /* TODO: Escribir variables de las tablas no globales ???? */
}

init_main:{
    fprintf(stderr, ";DEBUG: Writing beggining of main\n");
    escribir_inicio_main(yyout);
}

declaraciones:
    declaracion { fprintf(stderr, ";R2:\t<declaraciones> ::= <declaracion>\n"); }
|   declaracion declaraciones { fprintf(stderr, ";R3:\t<declaraciones> ::= <declaracion> <declaraciones>\n"); }
;

declaracion:
    clase identificadores TOK_PUNTOYCOMA { fprintf(stderr, ";R4:\t<declaracion> ::= <clase> <identificadores> ;\n"); }
;

clase:
    clase_escalar {
        clase_actual = SCALAR;
        fprintf(stderr, ";R5:\t<clase> ::= <clase_escalar>\n");
     }
|   clase_vector {
        clase_actual = VECTOR;
        fprintf(stderr, ";R7:\t<clase> ::= <clase_vector>\n");
    }
;

clase_escalar:
    tipo { fprintf(stderr, ";R9:\t<clase_escalar> ::= <tipo>\n"); }
;

tipo:
    TOK_INT {
        tipo_actual = INTEGER;
        fprintf(stderr, ";R10:\t<tipo> ::= int\n");
    }
|   TOK_BOOLEAN {
        tipo_actual = BOOLEAN;
        fprintf(stderr, ";R11:\t<tipo> ::= boolean\n");
    }
;

clase_vector:
    TOK_ARRAY tipo TOK_CORCHETEIZQUIERDO constante_entera TOK_CORCHETEDERECHO {
        vector_size = $4.int_value;
        fprintf(stderr, ";R15:\t<clase_vector> ::= array <tipo> [ <constante_entera> ]\n");
    }
;

identificadores:
    identificador { fprintf(stderr, ";R18:\t<identificadores> ::= <identificador>\n"); }
|   identificador TOK_COMA identificadores { fprintf(stderr, ";R19:\t<identificadores> ::= <identificador> , <identificadores>\n"); }
;

funciones:
    funcion funciones { fprintf(stderr, ";R20:\t<funciones> ::= <funcion> <funciones>\n"); }
|   %empty { fprintf(stderr, ";R21:\t<funciones> ::=\n"); }
;

funcion:
    TOK_FUNCTION tipo identificador TOK_PARENTESISIZQUIERDO parametros_funcion TOK_PARENTESISDERECHO TOK_LLAVEIZQUIERDA declaraciones_funcion sentencias TOK_LLAVEDERECHA { fprintf(stderr, ";R22:\t<funcion> ::= function <tipo> <identificador> ( <parametros_funcion> ) { <declaraciones_funcion> <sentencias> }\n"); }
;

parametros_funcion:
    parametro_funcion resto_parametros_funcion { fprintf(stderr, ";R23:\t<parametros_funcion> ::= <parametro_funcion> <resto_parametros_funcion>\n"); }
|   %empty { fprintf(stderr, ";R24:\t<parametros_funcion> ::=\n"); }
;

resto_parametros_funcion:
    TOK_PUNTOYCOMA parametro_funcion resto_parametros_funcion { fprintf(stderr, ";R25:\t<resto_parametros_funcion> ::= ; <parametro_funcion> <resto_parametros_funcion>\n"); }
|   %empty { fprintf(stderr, ";R26:\t<resto_parametros_funcion> ::=\n"); }
;

parametro_funcion:
    tipo identificador { fprintf(stderr, ";R27:\t<parametro_funcion> ::= <tipo> <identificador>\n"); }
;

declaraciones_funcion:
    declaraciones { fprintf(stderr, ";R28:\t<declaraciones_funcion> ::= <declaraciones>\n"); }
|   %empty { fprintf(stderr, ";R28:\t<declaraciones_funcion> ::=\n"); }
;

sentencias:
    sentencia { fprintf(stderr, ";R30:\t<sentencias> ::= <sentencia>\n"); }
|   sentencia sentencias { fprintf(stderr, ";R31:\t<sentencias> ::= <sentencia> <sentencias>\n"); }
;

sentencia:
    sentencia_simple TOK_PUNTOYCOMA { fprintf(stderr, ";R32:\t<sentencia> ::= <sentencia_simple> ;\n"); }
|   bloque { fprintf(stderr, ";R33:\t<sentencia> ::= <bloque>\n"); }
;

sentencia_simple:
    asignacion { fprintf(stderr, ";R34:\t<sentencia_simple> ::= <asignacion>\n"); }
|   lectura { fprintf(stderr, ";R35:\t<sentencia_simple> ::= <lectura>\n"); }
|   escritura { fprintf(stderr, ";R36:\t<sentencia_simple> ::= <escritura>\n"); }
|   retorno_funcion { fprintf(stderr, ";R38:\t<sentencia_simple> ::= <retorno_funcion>\n"); }
;

bloque:
    condicional { fprintf(stderr, ";R40:\t<bloque> ::= <condicional>\n"); }
|   bucle { fprintf(stderr, ";R41:\t<bloque> ::= <bucle>\n"); }
;

asignacion:
    TOK_IDENTIFICADOR TOK_ASIGNACION exp {
        int is_local = -1;
        Symbol *symb=NULL;
        symb = symb_tb_com_search(symb_tb, $1.lexeme, &is_local);
        if(!symb){
            /* TODO : Error -> variable no declarada */
            return -1;
        }
        if(symb->symb_cat == FUNCTION || symb->var_cat == VECTOR || symb->symb_type != $3.type){
            /* TODO : Error -> asignacion incompatible */
            return -1;
        }

        if(is_local){ /* Local variable */
            if(symb->symb_cat == PARAMETER){
                /* TODO : GENERACION */
            }
            else{
                /* TODO : GENERACION */
            }
        }
        else{
            asignar(yyout, symb->id, $3.is_address);
            fprintf(stderr, ";R43:\t<asignacion> ::= <identificador> = <exp>\n");
        }
    }
|   elemento_vector TOK_ASIGNACION exp { fprintf(stderr, ";R44:\t<asignacion> ::= <elemento_vector> = <exp>\n"); }
;

elemento_vector:
    identificador TOK_CORCHETEIZQUIERDO exp TOK_CORCHETEDERECHO { fprintf(stderr, ";R48:\t<elemento_vector> ::= <identificador>\n"); }
;

condicional:
    TOK_IF TOK_PARENTESISIZQUIERDO exp TOK_PARENTESISDERECHO TOK_LLAVEIZQUIERDA exp TOK_LLAVEDERECHA { fprintf(stderr, ";R50:\t<condicional> ::= if ( <exp> ) { <sentencias> }\n"); }
|   TOK_IF TOK_PARENTESISIZQUIERDO exp TOK_PARENTESISDERECHO TOK_LLAVEIZQUIERDA exp TOK_LLAVEDERECHA TOK_ELSE TOK_LLAVEIZQUIERDA exp TOK_LLAVEIZQUIERDA { fprintf(stderr, ";R50:\t<condicional> ::= if ( <exp> ) { <sentencias> } else { <sentencias> }\n"); }
;

bucle:
    TOK_WHILE TOK_PARENTESISIZQUIERDO exp TOK_PARENTESISDERECHO TOK_LLAVEIZQUIERDA sentencias TOK_LLAVEDERECHA { fprintf(stderr, ";R52:\t<bucle> ::= while ( <exp> ) { <sentencias> }\n"); }
;

lectura:
    TOK_SCANF identificador { fprintf(stderr, ";R54:\t<lectura> ::= scanf <identificador>\n"); }
;

escritura:
    TOK_PRINTF exp {
        // escribir_operando(yyout, $2.lexeme, $2.is_address); XXX: Ya lo está haciendo TOK_IDENTIFICADOR en <exp>
        escribir(yyout, $2.is_address, $2.type);
        fprintf(stderr, ";R56:\t<escritura> ::= printf <exp>\n");
    }
;

retorno_funcion:
    TOK_RETURN exp { fprintf(stderr, ";R61:\t<retorno_funcion> ::= return <exp>\n"); }
;

exp:
    exp TOK_MAS exp { fprintf(stderr, ";R72:\t<exp> ::= <exp> + <exp>\n"); }
|   exp TOK_MENOS exp { fprintf(stderr, ";R73:\t<exp> ::= <exp> - <exp>\n"); }
|   exp TOK_DIVISION exp { fprintf(stderr, ";R74:\t<exp> ::= <exp> / <exp>\n"); }
|   exp TOK_ASTERISCO exp { fprintf(stderr, ";R75:\t<exp> ::= <exp> * <exp>\n"); }
|   TOK_MENOS exp { fprintf(stderr, ";R76:\t<exp> ::= -<exp>\n"); }
|   exp TOK_AND exp { fprintf(stderr, ";R77:\t<exp> ::= <exp> && <exp>\n"); }
|   exp TOK_OR exp { fprintf(stderr, ";R78:\t<exp> ::= <exp> || <exp>\n"); }
|   TOK_NOT exp { fprintf(stderr, ";R79:\t<exp> ::= !<exp>\n"); }
|   TOK_IDENTIFICADOR {
        int is_local = -1;
        Symbol *symb=NULL;
        symb = symb_tb_com_search(symb_tb, $1.lexeme, &is_local);
        if(!symb){
            /* TODO : Error -> variable no declarada */
            return -1;
        }
        if(symb->symb_cat == FUNCTION || symb->var_cat == VECTOR){
            /* TODO : Error -> asignacion incompatible */
            return -1;
        }

        $$.type = symb->symb_type;
        $$.is_address = 1;

        escribir_operando(yyout, symb->id, 1);
        /* TODO : Escritura en ensamblador de la introduccion en la pila de la dirección del identificador: push dword  _$1.lexema */

        fprintf(stderr, ";R80:\t<exp> ::= <identificador>\n");
    }
|   constante {
        $$.type = $1.type;
        $$.is_address = $1.is_address;
        char buff[20];
        sprintf(buff, "%d", $1.int_value);
        escribir_operando(yyout, buff, $1.is_address); /*TODO: CODIFICAR TRUE Y FALSE ADECUADAMENTE O HACER UN IF*/
        fprintf(stderr, ";R81:\t<exp> ::= <constante>\n");
    }
|   TOK_PARENTESISIZQUIERDO exp TOK_PARENTESISDERECHO { fprintf(stderr, ";R82:\t<exp> ::= (<exp>)\n"); }
|   TOK_PARENTESISIZQUIERDO comparacion TOK_PARENTESISDERECHO { fprintf(stderr, ";R83:\t<exp> ::= (<comparacion>)\n"); }
|   elemento_vector { fprintf(stderr, ";R85:\t<exp> ::= <elemento_vector>\n"); }
|   identificador TOK_PARENTESISIZQUIERDO lista_expresiones TOK_PARENTESISDERECHO { fprintf(stderr, ";R88:\t<exp> ::= <identificador> ( <lista_expresiones> )\n"); }
;

lista_expresiones:
    exp resto_lista_expresiones { fprintf(stderr, ";R89:\t<lista_expresiones> ::= <exp> <resto_lista_expresiones>\n"); }
|   %empty { fprintf(stderr, ";R90:\t<lista_expresiones> ::=\n"); }
;

resto_lista_expresiones:
    TOK_COMA exp resto_lista_expresiones { fprintf(stderr, ";R91:\t<resto_lista_expresiones> ::= , <exp> <resto_lista_expresiones>\n"); }
|   %empty { fprintf(stderr, ";R92:\t<resto_lista_expresiones> ::=\n"); }
;

comparacion:
    exp TOK_IGUAL exp { fprintf(stderr, ";R93:\t<comparacion> ::= <exp> == <exp>\n"); }
|   exp TOK_DISTINTO exp { fprintf(stderr, ";R94:\t<comparacion> ::= <exp> != <exp>\n"); }
|   exp TOK_MENORIGUAL exp { fprintf(stderr, ";R95:\t<comparacion> ::= <exp> <= <exp>\n"); }
|   exp TOK_MAYORIGUAL exp { fprintf(stderr, ";R96:\t<comparacion> ::= <exp> >= <exp>\n"); }
|   exp TOK_MENOR exp { fprintf(stderr, ";R97:\t<comparacion> ::= <exp> < <exp>\n"); }
|   exp TOK_MAYOR exp { fprintf(stderr, ";R98:\t<comparacion> ::= <exp> > <exp>\n"); }
;

constante:
    constante_entera {
        $$.type = $1.type;
        $$.is_address = $1.is_address;
        fprintf(stderr, ";R99:\t<constante> ::= <constante_entera>\n");
    }
|   constante_logica { fprintf(stderr, ";R100:\t<constante> ::= <constante_logica>\n"); }
;

constante_logica:
    TOK_TRUE { fprintf(stderr, ";R102:\t<constante_logica> ::= true\n"); }
|   TOK_FALSE { fprintf(stderr, ";R103:\t<constante_logica> ::= false\n"); }
;

constante_entera: TOK_CONSTANTE_ENTERA {
    $$.type = INTEGER;
    $$.is_address = 0;
    $$.int_value = $1.int_value;
    fprintf(stderr, ";R104:\t<constante_entera> ::= TOK_CONSTANTE_ENTERA\n");
    /* TODO : Meter en la pila la constante -> ¿ operandoEnPilaAArgumento(yyout, $$.is_address) ? */
}
;

identificador: TOK_IDENTIFICADOR {
    int is_local = -1; /* To check if the ID is local or global */
    Symbol *symb=NULL;
    symb = symb_tb_com_search(symb_tb, $1.lexeme, &is_local);
    if((symb && symb->symb_cat == FUNCTION) || (symb && is_local)) {
        printf("****Error semantico en lin %lu: Declaracion duplicada.\n", nlines);
        return -1; /* constant ERROR ? */
    }
    /* Creating symbol */
    symb = (Symbol *) calloc(1, sizeof(Symbol));
    /* TODO : Error check ? */
    symb->id = (char *) calloc(strlen($1.lexeme)+1, sizeof(char));

    strcpy(symb->id, $1.lexeme);
    symb->symb_cat = VARIABLE;
    symb->symb_type = tipo_actual;
    symb->var_cat = clase_actual;

    if(clase_actual == VECTOR) {
        symb->len = vector_size;
        vector_size = 0; /* TODO: Esto hay que resetearlo? */
    } else {
        symb->len = 1;
    }

    /* TODO : Comprobar si es funcion */
    /* TODO :
    symb->value = 0;
    symb->len = 0;
    symb->num_param = 0;
    symb->pos = 0;
    symb->num_local_var = 0;
    */
    symb_tb_com_insert(symb_tb, symb);

    fprintf(stderr, ";R108:\t<identificador> ::= TOK_IDENTIFICADOR\n");
}
;

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
