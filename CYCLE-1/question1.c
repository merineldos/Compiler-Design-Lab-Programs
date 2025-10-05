#include <stdio.h>
#include <stdlib.h>

int no_of_states;

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

int main() {
    char state1, state2, inp;
    int end;

    printf("Enter total number of states: ");
    scanf("%d", &no_of_states);

    int closure[10][10];
    FILE *INPUT = fopen("automata.txt", "r");
    if(!INPUT) {
        printf("Error opening file!\n");
        return 1;
    }

    reset(closure);

    for(int i = 0; i < no_of_states; i++) {
        int state = i;
        while((end = fscanf(INPUT, " %c %c %c", &state1, &inp, &state2)) != EOF) {
            if(inp == 'e' && state == (state1 - '0')) {
                closure[i][state2 - '0'] = 1;
                state = state2 - '0';
            }
        }
        rewind(INPUT);
    }

    display(closure);

    fclose(INPUT);
    return 0;
}
