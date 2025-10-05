/* ast.h */
#ifndef AST_H
#define AST_H

typedef struct AST {
    char *nodeType;
    int value;
    struct AST *left;
    struct AST *right;
} AST;

#endif
