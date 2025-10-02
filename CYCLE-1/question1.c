#include <stdio.h>
#include <string.h>

char closure[10][10]; // To store closure for each state
int visited[10];      // To track visited states in closure expansion

// Recursively explore epsilon transitions
void epsilon_closure(char state[], int *count) {
    FILE *f = fopen("automata.txt", "r");
    if (!f) {
        printf("Error opening file.\n");
        return;
    }

    char from[10], arrow[10], to[10];
    while (fscanf(f, "%s %s %s", from, arrow, to) == 3) {
        if (strcmp(from, state) == 0 && strcmp(arrow, "e") == 0) {
            // Check if 'to' is already in closure
            int already = 0;
            for (int i = 0; i < *count; i++) {
                if (strcmp(closure[i], to) == 0) {
                    already = 1;
                    break;
                }
            }

            if (!already) {
                strcpy(closure[(*count)++], to);
                epsilon_closure(to, count);  // Recursive call
            }
        }
    }

    fclose(f);
}

int main() {
    int n;
    char states[10][10];

    printf("How many states? ");
    scanf("%d", &n);

    printf("Enter states: ");
    for (int i = 0; i < n; i++) {
        scanf("%s", states[i]);
    }

    for (int i = 0; i < n; i++) {
        int count = 0;
        strcpy(closure[count++], states[i]); // Start with itself
        epsilon_closure(states[i], &count);   // Recursively expand

        // Print closure
        printf("Epsilon closure of %s = { ", states[i]);
        for (int j = 0; j < count; j++) {
            printf("%s ", closure[j]);
        }
        printf("}\n");
    }

    return 0;
}