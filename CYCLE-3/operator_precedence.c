#include <stdio.h>
#include <string.h>

int main() {
    char stack[20], ip[20], opt[10][10], ter[10];
    int i, j, k, n, top = 0, col = 0, row = 0;

    // Initialize arrays
    for (i = 0; i < 20; i++) {
        stack[i] = '\0';
        ip[i] = '\0';
    }
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            opt[i][j] = '\0';
        }
    }

    // Input terminals
    printf("Enter the no.of terminals:\n");
    scanf("%d", &n);

    printf("\nEnter the terminals:\n");
    scanf("%s", ter);

    // Input operator precedence table
    printf("\nEnter the table values:\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("Enter the value for %c %c: ", ter[i], ter[j]);
            scanf(" %c", &opt[i][j]); // read single char
        }
    }

    // Print operator precedence table
    printf("\n**** OPERATOR PRECEDENCE TABLE ****\n");
    printf("\t");
    for (i = 0; i < n; i++) {
        printf("%c\t", ter[i]);
    }
    printf("\n");

    for (i = 0; i < n; i++) {
        printf("%c\t", ter[i]);
        for (j = 0; j < n; j++) {
            printf("%c\t", opt[i][j]); // now a single char
        }
        printf("\n");
    }

    // Parsing
    stack[top] = '$';
    printf("\nEnter the input string (end with $): ");
    scanf("%s", ip);

    i = 0;
    printf("\nSTACK\t\tINPUT STRING\t\tACTION\n");
    printf("%s\t\t%s\t\t", stack, ip);

    while (i <= strlen(ip)) {
        for (k = 0; k < n; k++) {
            if (stack[top] == ter[k]) col = k;
            if (ip[i] == ter[k]) row = k;
        }

        if ((stack[top] == '$') && (ip[i] == '$')) {
            printf("String is accepted\n");
            break;
        } 
        else if ((opt[col][row] == '<') || (opt[col][row] == '=')) { // compare single char
            stack[++top] = opt[col][row];
            stack[++top] = ip[i];
            printf("Shift %c", ip[i]);
            i++;
        } 
        else if (opt[col][row] == '>') {
            while (stack[top] != '<') {
                --top;
            }
            top = top - 1;
            printf("Reduce");
        } 
        else {
            printf("\nString is not accepted\n");
            break;
        }

        printf("\n");
        for (k = 0; k <= top; k++) {
            printf("%c", stack[k]);
        }
        printf("\t\t");
        for (k = i; k < strlen(ip); k++) {
            printf("%c", ip[k]);
        }
        printf("\t\t");
    }

    return 0;
}
