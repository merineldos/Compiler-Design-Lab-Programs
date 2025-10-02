%{
#include <stdio.h>
%}

%%

.*meri.*    { printf("Rejected: contains 'meri'\n"); }

.*          { printf("Accepted: %s\n", yytext); }

%%

int yywrap() {
    return 1;
}

int main() {
    yylex();
    return 0;
}
