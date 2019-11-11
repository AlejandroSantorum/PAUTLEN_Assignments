%{
#include "tokens.h"
size_t ncols=1;
size_t nlines=1;
%}
%option noyywrap

%%

"//".* { ncols += yyleng; }
" "+ { ncols += yyleng; }
"\t"+ { ncols += yyleng; }
"\n" { ncols = 1; nlines++; }
main { ncols += yyleng; return TOK_MAIN; }
int { ncols += yyleng; return TOK_INT; }
boolean { ncols += yyleng; return TOK_BOOLEAN; }
array { ncols += yyleng; return TOK_ARRAY; }
function { ncols += yyleng; return TOK_FUNCTION; }
if { ncols += yyleng; return TOK_IF; }
else { ncols += yyleng; return TOK_ELSE; }
while { ncols += yyleng; return TOK_WHILE; }
scanf { ncols += yyleng; return TOK_SCANF; }
printf { ncols += yyleng; return TOK_PRINTF; }
return { ncols += yyleng; return TOK_RETURN; }
";" { ncols += yyleng; return TOK_PUNTOYCOMA; }
"," { ncols += yyleng; return TOK_COMA; }
"(" { ncols += yyleng; return TOK_PARENTESISIZQUIERDO; }
")" { ncols += yyleng; return TOK_PARENTESISDERECHO; }
"[" { ncols += yyleng; return TOK_CORCHETEIZQUIERDO; }
"]" { ncols += yyleng; return TOK_CORCHETEDERECHO; }
"{" { ncols += yyleng; return TOK_LLAVEIZQUIERDA; }
"}" { ncols += yyleng; return TOK_LLAVEDERECHA; }
"=" { ncols += yyleng; return TOK_ASIGNACION; }
"+" { ncols += yyleng; return TOK_MAS; }
"-" { ncols += yyleng; return TOK_MENOS; }
"/" { ncols += yyleng; return TOK_DIVISION; }
"*" { ncols += yyleng; return TOK_ASTERISCO; }
"&&" { ncols += yyleng; return TOK_AND; }
"||" { ncols += yyleng; return TOK_OR; }
"!" { ncols += yyleng; return TOK_DISTINTO; }
"==" { ncols += yyleng; return TOK_IGUAL; }
"!=" { ncols += yyleng; return TOK_DISTINTO; }
"<=" { ncols += yyleng; return TOK_MENORIGUAL; }
">=" { ncols += yyleng; return TOK_MAYORIGUAL; }
"<" { ncols += yyleng; return TOK_MENOR; }
">" { ncols += yyleng; return TOK_MAYOR; }
true { ncols += yyleng; return TOK_TRUE; }
false { ncols += yyleng; return TOK_FALSE; }
[0-9]+ { ncols += yyleng; return TOK_CONSTANTE_ENTERA; }
[a-zA-Z]([0-9]|[a-zA-Z]){0,99} { ncols += yyleng; return TOK_IDENTIFICADOR; }
[a-zA-Z]([0-9]|[a-zA-Z]){100,} { return TOK_ERRORID; }
. { return TOK_ERROR; }
%%
