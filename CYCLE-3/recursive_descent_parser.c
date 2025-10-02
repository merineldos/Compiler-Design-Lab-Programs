#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char ip_sym[50];     // Input string
int ip_ptr = 0;      // Input pointer
char op[100];        // Current derivation
char tmp[100];       // Temporary for cleaning

void e(); 
void e_prime(); 
void t(); 
void t_prime(); 
void f();
void advance();

// --- Utility function to clean epsilon (remove 'e') ---
void clean_op() {
    int n = 0;
    for (int i = 0; i < strlen(op); i++)
        if (op[i] != 'e')
            tmp[n++] = op[i];
    tmp[n] = '\0';
    strcpy(op, tmp);
}

// --- Grammar Functions ---

void e() {
    strcpy(op, "TE'");
    printf("E=%-25s E->TE'\n", op);
    t();
    e_prime();
}

void e_prime() {
    clean_op();
    int pos = strchr(op, 'E') - op;   // Find E'
    if (ip_sym[ip_ptr] == '+') {
        // Replace E' → +TE'
        memmove(op + pos + 3, op + pos + 1, strlen(op) - pos);
        op[pos] = '+';
        op[pos+1] = 'T';
        op[pos+2] = 'E';
        op[pos+3] = '\'';
        printf("E=%-25s E'->+TE'\n", op);
        advance();
        t();
        e_prime();
    } else {
        op[pos] = 'e';
        printf("E=%-25s E'->e\n", op);
    }
}

void t() {
    clean_op();
    int pos = strchr(op, 'T') - op;
    // Replace T → FT'
    memmove(op + pos + 2, op + pos + 1, strlen(op) - pos);
    op[pos] = 'F';
    op[pos+1] = 'T';
    op[pos+2] = '\'';
    printf("E=%-25s T->FT'\n", op);
    f();
    t_prime();
}

void t_prime() {
    clean_op();
    int pos = strchr(op, 'T') - op;
    if (ip_sym[ip_ptr] == '*') {
        // Replace T' → *FT'
        memmove(op + pos + 3, op + pos + 1, strlen(op) - pos);
        op[pos] = '*';
        op[pos+1] = 'F';
        op[pos+2] = 'T';
        op[pos+3] = '\'';
        printf("E=%-25s T'->*FT'\n", op);
        advance();
        f();
        t_prime();
    } else {
        op[pos] = 'e';
        printf("E=%-25s T'->e\n", op);
    }
}

void f() {
    clean_op();
    int pos = strchr(op, 'F') - op;
    if (ip_sym[ip_ptr] == 'i') {
        op[pos] = 'i';
        printf("E=%-25s F->i\n", op);
        advance();
    } else if (ip_sym[ip_ptr] == '(') {
        advance();
        e();
        if (ip_sym[ip_ptr] == ')') {
            advance();
            // Replace F → (E)
            memmove(op + pos + 3, op + pos + 1, strlen(op) - pos);
            op[pos] = '(';
            op[pos+1] = 'E';
            op[pos+2] = ')';
            printf("E=%-25s F->(E)\n", op);
        } else {
            printf("\nSyntax Error: missing ')'\n");
            exit(1);
        }
    } else {
        printf("\nSyntax Error at %c\n", ip_sym[ip_ptr]);
        exit(1);
    }
}

void advance() {
    ip_ptr++;
}

// --- Main ---
int main() {
    printf("Grammar (no left recursion):\n");
    printf("E -> T E'\nE' -> + T E' | e\nT -> F T'\nT' -> * F T' | e\nF -> (E) | i\n");

    printf("\nEnter the input expression: ");
    scanf("%s", ip_sym);

    printf("\nDerivation steps:\n");
    e();

    if (ip_sym[ip_ptr] == '\0')
        printf("\nInput accepted \n");
    else
        printf("\nSyntax Error: Extra input remaining\n");

    return 0;
}
