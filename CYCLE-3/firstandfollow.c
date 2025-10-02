#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 20

int n;                      // number of productions
char prods[MAX][MAX];       // productions
char firsts[26][MAX];       // FIRST sets
char follows[26][MAX];      // FOLLOW sets
int is_first_done[26];      // flags for computed FIRST
int is_follow_done[26];     // flags for computed FOLLOW

// Function to check if symbol is terminal
int isTerminal(char c) {
    if (c < 'A' || c > 'Z')
        return 1;
    return 0;
}

// Add symbol to set (if not already present)
void addToSet(char set[], char c) {
    for (int i = 0; set[i] != '\0'; i++) {
        if (set[i] == c) return;  // already present
    }
    int len = strlen(set);
    set[len] = c;
    set[len + 1] = '\0';
}

// Compute FIRST of nonterminal
void first(char nonterm) {
    if (is_first_done[nonterm - 'A']) return;

    char curr_first[MAX] = "";
    for (int i = 0; i < n; i++) {
        if (prods[i][0] == nonterm) {
            int j = 2; // start after "A="
            int nullable = 1;

            while (prods[i][j] != '\0' && nullable) {
                nullable = 0;

                if (isTerminal(prods[i][j])) {
                    addToSet(curr_first, prods[i][j]);
                    break;
                } else {
                    char sym = prods[i][j];
                    if (!is_first_done[sym - 'A'])
                        first(sym);

                    // Copy FIRST(sym) into curr_first
                    for (int k = 0; firsts[sym - 'A'][k] != '\0'; k++) {
                        if (firsts[sym - 'A'][k] == 'e') {
                            nullable = 1;
                        } else {
                            addToSet(curr_first, firsts[sym - 'A'][k]);
                        }
                    }
                }
                j++;
            }
            if (nullable) {
                addToSet(curr_first, 'e');
            }
        }
    }

    strcpy(firsts[nonterm - 'A'], curr_first);
    is_first_done[nonterm - 'A'] = 1;
}

// Compute FOLLOW of nonterminal
void follow(char nonterm) {
    if (is_follow_done[nonterm - 'A']) return;

    char curr_follow[MAX] = "";

    // Start symbol gets '$'
    if (nonterm == prods[0][0]) {
        addToSet(curr_follow, '$');
    }

    for (int i = 0; i < n; i++) {
        for (int j = 2; prods[i][j] != '\0'; j++) {
            if (prods[i][j] == nonterm) {
                int k = j + 1;
                int nullable = 1;

                while (prods[i][k] != '\0' && nullable) {
                    nullable = 0;

                    if (isTerminal(prods[i][k])) {
                        addToSet(curr_follow, prods[i][k]);
                        break;
                    } else {
                        char sym = prods[i][k];
                        if (!is_first_done[sym - 'A'])
                            first(sym);

                        for (int x = 0; firsts[sym - 'A'][x] != '\0'; x++) {
                            if (firsts[sym - 'A'][x] == 'e') {
                                nullable = 1;
                            } else {
                                addToSet(curr_follow, firsts[sym - 'A'][x]);
                            }
                        }
                    }
                    k++;
                }

                if (prods[i][k] == '\0' || nullable) {
                    if (prods[i][0] != nonterm) {
                        if (!is_follow_done[prods[i][0] - 'A'])
                            follow(prods[i][0]);

                        for (int x = 0; follows[prods[i][0] - 'A'][x] != '\0'; x++) {
                            addToSet(curr_follow, follows[prods[i][0] - 'A'][x]);
                        }
                    }
                }
            }
        }
    }

    strcpy(follows[nonterm - 'A'], curr_follow);
    is_follow_done[nonterm - 'A'] = 1;
}

// Main function
int main() {
    printf("Enter the number of productions: ");
    scanf("%d", &n);

    printf("Enter the productions (like E=E+T, E=T):\n");
    for (int i = 0; i < n; i++) {
        scanf("%s", prods[i]);
    }

    // Initialize
    for (int i = 0; i < 26; i++) {
        is_first_done[i] = 0;
        is_follow_done[i] = 0;
        firsts[i][0] = '\0';
        follows[i][0] = '\0';
    }

    // Compute FIRST and FOLLOW
    for (int i = 0; i < n; i++) {
        if (!is_first_done[prods[i][0] - 'A'])
            first(prods[i][0]);
    }
    for (int i = 0; i < n; i++) {
        if (!is_follow_done[prods[i][0] - 'A'])
            follow(prods[i][0]);
    }

    // Print results
    printf("\nFIRST sets:\n");
    for (int i = 0; i < 26; i++) {
        if (is_first_done[i]) {
            printf("%c : { %s }\n", i + 'A', firsts[i]);
        }
    }

    printf("\nFOLLOW sets:\n");
    for (int i = 0; i < 26; i++) {
        if (is_follow_done[i]) {
            printf("%c : { %s }\n", i + 'A', follows[i]);
        }
    }

    return 0;
}
