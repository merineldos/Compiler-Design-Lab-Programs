#include <stdio.h>
#include <string.h>

#define MAX 20

int n, a, s, f[MAX], t, T[MAX][MAX][MAX], Tcount[MAX][MAX];
int DFA[MAX][MAX], dfaCount = 0;
char alpha[MAX];

int main() {
    int i, j, k, from, to;
    char sym;

    printf("States, Alphabets, Start: ");
    scanf("%d %d %d", &n, &a, &s);

    printf("Alphabets: ");
    for(i = 0; i < a; i++) scanf(" %c", &alpha[i]);

    int finCount;
    printf("Final count & states: ");
    scanf("%d", &finCount);
    for(i = 0; i < finCount; i++) scanf("%d", &f[i]);

    printf("Transitions count: ");
    scanf("%d", &t);
    printf("Transitions (from symbol to):\n");
    for(i = 0; i < t; i++) {
        scanf("%d %c %d", &from, &sym, &to);
        for(j = 0; j < a; j++)
            if(alpha[j] == sym) break;
        T[from][j][Tcount[from][j]++] = to;
    }

    // Init DFA with start state
    DFA[0][s] = 1;
    dfaCount = 1;

    for(i = 0; i < dfaCount; i++) {
        for(j = 0; j < a; j++) {
            int new[MAX] = {0};
            for(k = 0; k < n; k++) {
                if(DFA[i][k]) {
                    for(int l = 0; l < Tcount[k][j]; l++)
                        new[T[k][j][l]] = 1;
                }
            }

            // Check if new state exists
            int found = -1;
            for(int m = 0; m < dfaCount; m++) {
                if(memcmp(DFA[m], new, sizeof(new)) == 0)
                    found = m;
            }

            if(found == -1) {
                memcpy(DFA[dfaCount], new, sizeof(new));
                found = dfaCount++;
            }
        }
    }

    printf("\nDFA States:\n");
    for(i = 0; i < dfaCount; i++) {
        printf("q%d = {", i);
        for(j = 0; j < n; j++)
            if(DFA[i][j]) printf("q%d ", j);
        printf("}\n");
    }

    printf("\nFinal DFA States: ");
    for(i = 0; i < dfaCount; i++) {
        for(j = 0; j < n; j++)
            for(k = 0; k < finCount; k++)
                if(DFA[i][j] && f[k] == j) {
                    printf("q%d ", i);
                    j = n; // break out
                }
    }

    return 0;
}

