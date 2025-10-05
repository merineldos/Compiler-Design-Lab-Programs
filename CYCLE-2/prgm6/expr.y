
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

int yylex(void);        // Lexer function
int yyerror(const char *s); // Error handler
/*
typedef struct AST {
    char *nodeType;      // Type of node: "+", "-", "*", "/", "NUM", "NEG"
    int value;           // Only used for numbers
    struct AST *left;    // Left child
    struct AST *right;   // Right child
} AST;
*/

/* Helper function definitions */
AST* makeNode(char *type, AST *l, AST *r) {
    AST *n = malloc(sizeof(AST));
    n->nodeType = strdup(type);
    n->value = 0;
    n->left = l;
    n->right = r;
    return n;
}

AST* makeNum(int val) {
    AST *n = malloc(sizeof(AST));
    n->nodeType = strdup("NUM");
    n->value = val;
    n->left = n->right = NULL;
    return n;
}

void inorderAST(AST *root) {
    if (!root) return;
    inorderAST(root->left);
    if (strcmp(root->nodeType, "NUM") == 0)
        printf("%d", root->value);
    else if (strcmp(root->nodeType, "NEG") == 0)
        printf("-");
    else
        printf("%s", root->nodeType);
    inorderAST(root->right);
}
void preorderAST(AST *root) {
    if (!root) return;
   
    if (strcmp(root->nodeType, "NUM") == 0)
        printf("%d", root->value);
    else if (strcmp(root->nodeType, "NEG") == 0)
        printf("-");
    else
        printf("%s", root->nodeType);
    preorderAST(root->left);
    preorderAST(root->right);
}
void postorderAST(AST *root) {
     if (!root) return;
    postorderAST(root->left);
    postorderAST(root->right);
    if (strcmp(root->nodeType, "NUM") == 0)
        printf("%d", root->value);
    else if (strcmp(root->nodeType, "NEG") == 0)
        printf("-");
    else
        printf("%s", root->nodeType);
   
}


void printAST(AST *root, int lvl) {
    if (!root) return;
    for (int i = 0; i < lvl; i++) printf("  ");
    if (strcmp(root->nodeType, "NUM") == 0)
        printf("NUM(%d)\n", root->value);
    else
        printf("%s\n", root->nodeType);
    printAST(root->left, lvl + 1);
    printAST(root->right, lvl + 1);
}

int evalAST(AST *root) {
    if (!root) return 0;
    if (strcmp(root->nodeType, "NUM") == 0) return root->value;
    if (strcmp(root->nodeType, "+") == 0) return evalAST(root->left) + evalAST(root->right);
    if (strcmp(root->nodeType, "-") == 0) return evalAST(root->left) - evalAST(root->right);
    if (strcmp(root->nodeType, "*") == 0) return evalAST(root->left) * evalAST(root->right);
    if (strcmp(root->nodeType, "/") == 0) return evalAST(root->left) / evalAST(root->right);
    if (strcmp(root->nodeType, "NEG") == 0) return -evalAST(root->left);
    return 0;
}
%}
%union {int num; AST *node; }
%token <num> NUMBER
%type <node> expr
%left '+' '-'
%left '*' '/'
%left UMINUS

%%
start:
    expr {
        printf("\nAbstract Syntax Tree:\n");
        printAST($1, 0);
        printf("\nInorder Traversal of Abstract Syntax Tree:\n");
        inorderAST($1);
        printf("\nPreorder Traversal of Abstract Syntax Tree:\n");
        preorderAST($1);
        printf("\nPostorder Traversal of Abstract Syntax Tree:\n");
        postorderAST($1);
        printf("\nResult: %d\n", evalAST($1));
    }
;

expr:
      expr '+' expr     { $$ = makeNode("+", $1, $3); }
    | expr '-' expr     { $$ = makeNode("-", $1, $3); }
    | expr '*' expr     { $$ = makeNode("*", $1, $3); }
    | expr '/' expr     { $$ = makeNode("/", $1, $3); }
    | '-' expr %prec UMINUS  { $$ = makeNode("NEG", $2, NULL); }
    | '(' expr ')'      { $$ = $2; }
    | NUMBER            { $$ = makeNum($1); }
;
%%

int main() {
    printf("Enter arithmetic expression:\n");
    yyparse();
    return 0;
}

int yyerror(const char *s) {
    printf("Error: %s\n", s);
    return 0;
}
