/*
    Authors:
            - Alejandro Santorum Varela
            - Rafael Sanchez Sanchez
    File: alfa.c
*/
#include <stdio.h>
#include <stdlib.h>

#include "generacion.h"
#include "symbol_table_com.h"

extern int yyparse(void);
extern FILE *yyin;
extern FILE* yyout;

#define ERROR -1

int main(int argc, char const *argv[]) {
    if(argc < 3){
        printf("Faltan parametros de entrada: ./pruebaSintactico <fichero-entrada> <fichero-salida>\n");
        return ERROR;
    }

    yyin = fopen(argv[1], "r");
    if(!yyin){
      printf("Error: Unable to open yyin\n");
      return ERROR;
    }

    yyout = fopen(argv[2], "w");
    if(!yyout){
      printf("Error: Unable to open output file\n");
      return ERROR;
    }

    yyparse();

    fclose(yyout);
    fclose(yyin);
    return 0;
}
