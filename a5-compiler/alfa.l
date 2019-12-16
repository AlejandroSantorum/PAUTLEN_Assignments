%{

#include "alfa.h"
#include "y.tab.h"

size_t ncols=1;
size_t nlines=1;
int is_morpho;

%}


%option noinput
%option nounput
%option noyywrap

%%

"//".* { ncols += yyleng; }
" "+ { ncols += yyleng; }
"\t"+ { ncols += yyleng; }
"\n" { ncols = 1; nlines++; }
main { ncols += yyleng; fprintf(stderr, ";D:\t%s\n", yytext); return TOK_MAIN; }
int { ncols += yyleng; fprintf(stderr, ";D:\t%s\n", yytext); return TOK_INT; }
boolean { ncols += yyleng; fprintf(stderr, ";D:\t%s\n", yytext); return TOK_BOOLEAN; }
array { ncols += yyleng; fprintf(stderr, ";D:\t%s\n", yytext); return TOK_ARRAY; }
function { ncols += yyleng; fprintf(stderr, ";D:\t%s\n", yytext); return TOK_FUNCTION; }
if { ncols += yyleng; fprintf(stderr, ";D:\t%s\n", yytext); return TOK_IF; }
else { ncols += yyleng; fprintf(stderr, ";D:\t%s\n", yytext); return TOK_ELSE; }
while { ncols += yyleng; fprintf(stderr, ";D:\t%s\n", yytext); return TOK_WHILE; }
scanf { ncols += yyleng; fprintf(stderr, ";D:\t%s\n", yytext); return TOK_SCANF; }
printf { ncols += yyleng; fprintf(stderr, ";D:\t%s\n", yytext); return TOK_PRINTF; }
return { ncols += yyleng; fprintf(stderr, ";D:\t%s\n", yytext); return TOK_RETURN; }
";" { ncols += yyleng; fprintf(stderr, ";D:\t%s\n", yytext); return TOK_PUNTOYCOMA; }
"," { ncols += yyleng; fprintf(stderr, ";D:\t%s\n", yytext); return TOK_COMA; }
"(" { ncols += yyleng; fprintf(stderr, ";D:\t%s\n", yytext); return TOK_PARENTESISIZQUIERDO; }
")" { ncols += yyleng; fprintf(stderr, ";D:\t%s\n", yytext); return TOK_PARENTESISDERECHO; }
"[" { ncols += yyleng; fprintf(stderr, ";D:\t%s\n", yytext); return TOK_CORCHETEIZQUIERDO; }
"]" { ncols += yyleng; fprintf(stderr, ";D:\t%s\n", yytext); return TOK_CORCHETEDERECHO; }
"{" { ncols += yyleng; fprintf(stderr, ";D:\t%s\n", yytext); return TOK_LLAVEIZQUIERDA; }
"}" { ncols += yyleng; fprintf(stderr, ";D:\t%s\n", yytext); return TOK_LLAVEDERECHA; }
"=" { ncols += yyleng; fprintf(stderr, ";D:\t%s\n", yytext); return TOK_ASIGNACION; }
"+" { ncols += yyleng; fprintf(stderr, ";D:\t%s\n", yytext); return TOK_MAS; }
"-" { ncols += yyleng; fprintf(stderr, ";D:\t%s\n", yytext); return TOK_MENOS; }
"/" { ncols += yyleng; fprintf(stderr, ";D:\t%s\n", yytext); return TOK_DIVISION; }
"*" { ncols += yyleng; fprintf(stderr, ";D:\t%s\n", yytext); return TOK_ASTERISCO; }
"&&" { ncols += yyleng; fprintf(stderr, ";D:\t%s\n", yytext); return TOK_AND; }
"||" { ncols += yyleng; fprintf(stderr, ";D:\t%s\n", yytext); return TOK_OR; }
"!" { ncols += yyleng; fprintf(stderr, ";D:\t%s\n", yytext); return TOK_NOT; }
"==" { ncols += yyleng; fprintf(stderr, ";D:\t%s\n", yytext); return TOK_IGUAL; }
"!=" { ncols += yyleng; fprintf(stderr, ";D:\t%s\n", yytext); return TOK_DISTINTO; }
"<=" { ncols += yyleng; fprintf(stderr, ";D:\t%s\n", yytext); return TOK_MENORIGUAL; }
">=" { ncols += yyleng; fprintf(stderr, ";D:\t%s\n", yytext); return TOK_MAYORIGUAL; }
"<" { ncols += yyleng; fprintf(stderr, ";D:\t%s\n", yytext); return TOK_MENOR; }
">" { ncols += yyleng; fprintf(stderr, ";D:\t%s\n", yytext); return TOK_MAYOR; }
true { ncols += yyleng; fprintf(stderr, ";D:\t%s\n", yytext); return TOK_TRUE; }
false { ncols += yyleng; fprintf(stderr, ";D:\t%s\n", yytext); return TOK_FALSE; }

[0-9]+ {
    ncols += yyleng;
    yylval.attributes.int_value = atoi(yytext);
    fprintf(stderr, ";D:\t%s\n", yytext);
    return TOK_CONSTANTE_ENTERA;
}

[a-zA-Z]([0-9]|[a-zA-Z]){0,99} {
    ncols += yyleng;
    strcpy(yylval.attributes.lexeme, yytext);
    fprintf(stderr, ";D:\t%s\n", yytext);
    return TOK_IDENTIFICADOR;
}

[a-zA-Z]([0-9]|[a-zA-Z]){100,} {
    printf("****Error en [lin %lu, col %lu]: identificador demasiado largo (%s)\n", nlines, ncols, yytext);
    is_morpho=1;
    return -1;
}

. {printf("****Error en [lin %lu, col %lu]: simbolo no permitido (%s)\n", nlines, ncols, yytext);  ncols++; is_morpho=1; return -2; }

%%
