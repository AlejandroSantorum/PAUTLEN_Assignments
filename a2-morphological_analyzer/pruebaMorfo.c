/*
    Authors:
            - Alejandro Santorum Varela
            - Rafael Sanchez Sanchez
    File: pruebaMorfo.c
*/
#include <stdio.h>
#include <stdlib.h>
#include "tokens.h"

extern int yylex(void);
extern char *yytext;
extern size_t yyleng;
extern FILE *yyin;
extern size_t ncols;
extern size_t nlines;

#define ERROR -1

int main(int argc, char const *argv[]) {
    int ret;
    FILE *fp;
    if(argc < 3){
        printf("Faltan parametros de entrada: ./pruebaMorfo <fichero-entrada> <fichero-salida>\n");
        return ERROR;
    }

    yyin = fopen(argv[1], "r");
    if(!yyin){
      printf("Error: Unable to open yyin\n");
      return ERROR;
    }

    fp = fopen(argv[2], "w");
    if(!fp){
      printf("Error: Unable to open output file\n");
      return ERROR;
    }

    while (ret = yylex()) {
        switch (ret) {
            case TOK_MAIN:
                fprintf(fp, "TOK_MAIN\t%d\t%s\n", ret, yytext);
            break;
            case TOK_INT:
                fprintf(fp, "TOK_INT\t%d\t%s\n", ret, yytext);
            break;
            case TOK_BOOLEAN:
                fprintf(fp, "TOK_BOOLEAN\t%d\t%s\n", ret, yytext);
            break;
            case TOK_ARRAY:
                fprintf(fp, "TOK_ARRAY\t%d\t%s\n", ret, yytext);
            break;
            case TOK_FUNCTION:
                fprintf(fp, "TOK_FUNCTION\t%d\t%s\n", ret, yytext);
            break;
            case TOK_IF:
                fprintf(fp, "TOK_IF\t%d\t%s\n", ret, yytext);
            break;
            case TOK_ELSE:
                fprintf(fp, "TOK_ELSE\t%d\t%s\n", ret, yytext);
            break;
            case TOK_WHILE:
                fprintf(fp, "TOK_WHILE\t%d\t%s\n", ret, yytext);
            break;
            case TOK_SCANF:
                fprintf(fp, "TOK_SCANF\t%d\t%s\n", ret, yytext);
            break;
            case TOK_PRINTF:
                fprintf(fp, "TOK_PRINTF\t%d\t%s\n", ret, yytext);
            break;
            case TOK_RETURN:
                fprintf(fp, "TOK_RETURN\t%d\t%s\n", ret, yytext);
            break;
            case TOK_PUNTOYCOMA:
                fprintf(fp, "TOK_PUNTOYCOMA\t%d\t%s\n", ret, yytext);
            break;
            case TOK_COMA:
                fprintf(fp, "TOK_COMA\t%d\t%s\n", ret, yytext);
            break;
            case TOK_PARENTESISDERECHO:
                fprintf(fp, "TOK_PARENTESISDERECHO\t%d\t%s\n", ret, yytext);
            break;
            case TOK_PARENTESISIZQUIERDO:
                fprintf(fp, "TOK_PARENTESISIZQUIERDO\t%d\t%s\n", ret, yytext);
            break;
            case TOK_CORCHETEDERECHO:
                fprintf(fp, "TOK_CORCHETEDERECHO\t%d\t%s\n", ret, yytext);
            break;
            case TOK_CORCHETEIZQUIERDO:
                fprintf(fp, "TOK_CORCHETEIZQUIERDO\t%d\t%s\n", ret, yytext);
            break;
            case TOK_LLAVEIZQUIERDA:
                fprintf(fp, "TOK_LLAVEIZQUIERDA\t%d\t%s\n", ret, yytext);
            break;
            case TOK_LLAVEDERECHA:
                fprintf(fp, "TOK_LLAVEDERECHA\t%d\t%s\n", ret, yytext);
            break;
            case TOK_ASIGNACION:
                fprintf(fp, "TOK_ASIGNACION\t%d\t%s\n", ret, yytext);
            break;
            case TOK_MAS:
                fprintf(fp, "TOK_MAS\t%d\t%s\n", ret, yytext);
            break;
            case TOK_MENOS:
                fprintf(fp, "TOK_MENOS\t%d\t%s\n", ret, yytext);
            break;
            case TOK_DIVISION:
                fprintf(fp, "TOK_DIVISION\t%d\t%s\n", ret, yytext);
            break;
            case TOK_ASTERISCO:
                fprintf(fp, "TOK_ASTERISCO\t%d\t%s\n", ret, yytext);
            break;
            case TOK_AND:
                fprintf(fp, "TOK_AND\t%d\t%s\n", ret, yytext);
            break;
            case TOK_OR:
                fprintf(fp, "TOK_OR\t%d\t%s\n", ret, yytext);
            break;
            case TOK_NOT:
                fprintf(fp, "TOK_NOT\t%d\t%s\n", ret, yytext);
            break;
            case TOK_IGUAL:
                fprintf(fp, "TOK_IGUAL\t%d\t%s\n", ret, yytext);
            break;
            case TOK_DISTINTO:
                fprintf(fp, "TOK_DISTINTO\t%d\t%s\n", ret, yytext);
            break;
            case TOK_CONSTANTE_ENTERA:
                fprintf(fp, "TOK_CONSTANTE_ENTERA\t%d\t%s\n", ret, yytext);
            break;
            case TOK_MENORIGUAL:
                fprintf(fp, "TOK_MENORIGUAL\t%d\t%s\n", ret, yytext);
            break;
            case TOK_MAYORIGUAL:
                fprintf(fp, "TOK_MAYORIGUAL\t%d\t%s\n", ret, yytext);
            break;
            case TOK_MENOR:
                fprintf(fp, "TOK_MENOR\t%d\t%s\n", ret, yytext);
            break;
            case TOK_MAYOR:
                fprintf(fp, "TOK_MAYOR\t%d\t%s\n", ret, yytext);
            break;
            case TOK_IDENTIFICADOR:
                fprintf(fp, "TOK_IDENTIFICADOR\t%d\t%s\n", ret, yytext);
            break;
            case TOK_TRUE:
                fprintf(fp, "TOK_TRUE\t%d\t%s\n", ret, yytext);
            break;
            case TOK_FALSE:
                fprintf(fp, "TOK_FALSE\t%d\t%s\n", ret, yytext);
            break;
            case TOK_ERROR:
                fprintf(stderr, "****Error en [lin %lu, col %lu]: simbolo no permitido (%s)\n", nlines, ncols, yytext);
                fclose(fp);
                fclose(yyin);
                exit(0);
            break;
            case TOK_ERRORID:
                fprintf(stderr, "****Error en [lin %lu, col %lu]: identificador demasiado largo (%s)\n", nlines, ncols, yytext);
                fclose(fp);
                fclose(yyin);
                exit(0);
            break;
            default:
                fprintf(stderr, "Something went wrong\n");
        }
    }
    fclose(fp);
    fclose(yyin);
    return 0;
}
