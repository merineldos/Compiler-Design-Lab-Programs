#include <stdio.h>
#include <stdlib.h>

int no_of_states;
int no_of_symbols;

void reset(int closure[10][10]) {
    for(int i = 0; i < no_of_states; i++)
        for(int j = 0; j < no_of_states; j++)
            if (i == j)
                closure[i][j] = 1;
            else
                closure[i][j]=0;
}

void display(int closure[10][10]) {
    printf("Epsilon closure of all states:\n");
    for(int i = 0; i < no_of_states; i++) {
        printf("q%d: {", i);
        for(int j = 0; j < no_of_states; j++)
            if(closure[i][j] == 1)
                printf("q%d, ", j);
        printf("}\n");
    }
}

void epsilon_closure(int closure[10][10], FILE *input_file) {
    char state1, state2, inp;
    int end;

    for(int i = 0; i < no_of_states; i++) {
        int state = i;
        while((end = fscanf(input_file, " %c %c %c", &state1, &inp, &state2)) != EOF) {
            if(inp == 'e' && state == (state1 - '0')) {
                closure[i][state2 - '0'] = 1;
                state = state2 - '0';
            }
        }
        rewind(input_file);
    }
}

int main() {
    printf("Enter total number of states: ");
    scanf("%d", &no_of_states);

    printf("Enter number of input symbols (excluding epsilon): ");
    scanf("%d", &no_of_symbols);

    int closure[10][10];
    reset(closure);

    FILE *input_file = fopen("automata.txt", "r");
    if(!input_file) {
        printf("Error opening file!\n");
        return 1;
    }
    epsilon_closure(closure, input_file);
    display(closure);

    
    int nfa[10][10][10] = {0}; // nfa[state][symbol][next_state]
    char state1, state2, inp;
    int end;

    while((end = fscanf(input_file, " %c %c %c", &state1, &inp, &state2)) != EOF) {
    if(inp != 'e') { 
        int from = state1 - '0';
        int sym = inp - 'a'; 
        int to = state2 - '0';

        // For every state that can reach "from" via epsilon
        for(int s = 0; s < no_of_states; s++) {
            if(closure[s][from]) {
                // Add epsilon-closure of "to" into transition table
                for(int k = 0; k < no_of_states; k++) {
                    if(closure[to][k]) {
                        nfa[s][sym][k] = 1;
                    }
                }
            }
        }
    }
}
    // Step 3: Display resulting NFA
    printf("\nNFA transition table (after removing epsilon transitions):\n");
    for(int i = 0; i < no_of_states; i++) {
        for(int j = 0; j < no_of_symbols; j++) {
            printf("From q%d on %c: {", i, 'a' + j);
            for(int k = 0; k < no_of_states; k++)
                if(nfa[i][j][k])
                    printf("q%d, ", k);
            printf("}\n");
        }
    }

    fclose(input_file);
    return 0;
}
