#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATES 20
#define MAX_SYMBOLS 10

int dfa[MAX_STATES][MAX_SYMBOLS];
int final[MAX_STATES];
int num_states, num_symbols;
char symbols[MAX_SYMBOLS];
int distinguish[MAX_STATES][MAX_STATES];

int get_symbol_index(char sym) {
    for (int i = 0; i < num_symbols; i++)
        if (symbols[i] == sym)
            return i;
    return -1;
}

void minimize_dfa() {
    int i, j, k;

    // Step 1: Initialize distinguish table
    for (i = 0; i < num_states; i++) {
        for (j = 0; j < i; j++) {
            if ((final[i] && !final[j]) || (!final[i] && final[j]))
                distinguish[i][j] = 1;  // Mark distinguishable
            else
                distinguish[i][j] = 0;
        }
    }

    // Step 2: Iteratively mark distinguishable pairs
    int changed = 1;
    while (changed) {
        changed = 0;
        for (i = 0; i < num_states; i++) {
            for (j = 0; j < i; j++) {
                if (distinguish[i][j]) continue;
                for (k = 0; k < num_symbols; k++) {
                    int a = dfa[i][k];
                    int b = dfa[j][k];
                    if (a == -1 || b == -1) continue; // skip missing transitions
                    if (a < b) {
                        if (distinguish[b][a]) { 
                            distinguish[i][j] = 1; 
                            changed = 1; 
                            break; 
                        }
                    } else {
                        if (distinguish[a][b]) { 
                            distinguish[i][j] = 1; 
                            changed = 1; 
                            break; 
                        }
                    }
                }
            }
        }
    }

    // Step 3: Group equivalent states
    int groups[MAX_STATES], group_id = 0, printed[MAX_STATES] = {0};

    // Initialize groups
    for (i = 0; i < num_states; i++) 
        groups[i] = -1;

    // Group equivalent states
    for (i = 0; i < num_states; i++) {
        if (groups[i] == -1) {
            groups[i] = group_id;
            for (j = i + 1; j < num_states; j++) {
                if (!distinguish[i > j ? i : j][i < j ? i : j]) {
                    groups[j] = group_id;
                }
            }
            group_id++;
        }
    }

    // Step 4: Print minimized DFA
    printf("\nMinimized DFA:\n");
    printf("Total states: %d\n", group_id);

    printf("State\t");
    for (i = 0; i < num_symbols; i++)
        printf("%c\t", symbols[i]);
    printf("\n");

    for (i = 0; i < group_id; i++) {
        printf("Q%d\t", i);
        for (j = 0; j < num_symbols; j++) {
            int target = -1;
            for (k = 0; k < num_states; k++) {
                if (groups[k] == i && dfa[k][j] != -1) {
                    target = groups[dfa[k][j]];
                    break;
                }
            }
            if (target != -1)
                printf("Q%d\t", target);
            else
                printf("-\t");
        }
        printf("\n");
    }

    // Step 5: Print final states
    printf("Final states: ");
    for (i = 0; i < num_states; i++) {
        if (final[i] && !printed[groups[i]]) {
            printf("Q%d ", groups[i]);
            printed[groups[i]] = 1;
        }
    }
    printf("\n");
}  


int main() {
    printf("Enter number of states in DFA: ");
    scanf("%d", &num_states);

    printf("Enter number of input symbols: ");
    scanf("%d", &num_symbols);

    printf("Enter input symbols:\n");
    for (int i = 0; i < num_symbols; i++)
        scanf(" %c", &symbols[i]);

    // Initialize DFA table with -1
    for (int i = 0; i < MAX_STATES; i++)
        for (int j = 0; j < MAX_SYMBOLS; j++)
            dfa[i][j] = -1;

    FILE *fp = fopen("dfa.txt", "r");
    if (!fp) {
        printf("Cannot open file dfa.txt\n");
        return 1;
    }

    int state1, state2;
    char inp;
    while (fscanf(fp, "%d %c %d", &state1, &inp, &state2) != EOF) {
        int sym_index = get_symbol_index(inp);
        if (sym_index == -1) {
            printf("Unknown input symbol %c\n", inp);
            return 1;
        }
        dfa[state1][sym_index] = state2;
    }
    fclose(fp);

    printf("Enter number of final states: ");
    int n;
    scanf("%d", &n);

    printf("Enter final states: ");
    for (int i = 0; i < n; i++) {
        int f;
        scanf("%d", &f);
        final[f] = 1;
    }

    minimize_dfa();
    return 0;
}
