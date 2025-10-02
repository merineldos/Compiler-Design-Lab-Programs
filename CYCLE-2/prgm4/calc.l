%{
#include "y.tab.h"
#include <stdio.h>
%}

%option noyywrap

%%

[0-9]+           { yylval = atoi(yytext); return NUMBER; }   // integers
[ \t]            ;   /* ignore spaces and tabs */
\n               { return '\n'; }                            // newline ends expression
"+"              { return PLUS; }
"-"              { return MINUS; }
"*"              { return MUL; }
"/"              { return DIV; }
"("              { return LPAREN; }
")"              { return RPAREN; }

. { printf("Invalid character: %s\n", yytext); }

%%
