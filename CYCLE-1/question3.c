//nfa to dfa conversion using subset construction

#include <stdio.h>
#include <string.h>
#define MAX 10

int n_states, n_symbols;
char symbols[MAX];

// NFA transitions
int nfa[MAX][MAX][MAX];
int nfa_count[MAX][MAX];

// DFA states as lists of NFA states
int dfa[MAX][MAX];
int dfa_size[MAX];
int dfa_count = 0;

// DFA transitions: dfa_trans[dfa_state][symbol] = next_dfa_state
int dfa_trans[MAX][MAX];

// Check if DFA state already exists
int exists(int new_state[], int size){
    for(int i=0;i<dfa_count;i++){
        if(dfa_size[i] != size) continue;
        int same=1;
        for(int j=0;j<size;j++)
            if(dfa[i][j] != new_state[j]) {same=0; break;}
        if(same) return i;
    }
    return -1;
}
int main() {
    printf("Enter number of states: ");
    scanf("%d",&n_states);

    printf("Enter number of symbols: ");
    scanf("%d",&n_symbols);

    printf("Enter symbols: ");
    for(int i=0;i<n_symbols;i++) scanf(" %c",&symbols[i]);

    FILE *fp = fopen("nfa.txt","r");
    if(!fp){ 
        printf("Cannot open file\n"); 
        return 0; 
    }

    int from, to;
    char sym;
    while(fscanf(fp,"%d %c %d",&from,&sym,&to) != EOF){
        int s;
        for(s=0;s<n_symbols;s++) if(symbols[s]==sym) break;
        
        nfa[from][s][nfa_count[from][s]] = to;
        nfa_count[from][s]++;
    }
    fclose(fp);

    // 3. Initialize DFA with start state {0}
    dfa[0][0] = 0; 
    dfa_size[0]=1; 
    dfa_count=1;

    // 4. Subset construction
    for(int i=0;i<dfa_count;i++){
        for(int s=0;s<n_symbols;s++){
            int new_state[MAX], new_size=0;

            // Collect all NFA states reachable from current DFA state on symbol s
            for(int j=0;j<dfa_size[i];j++){
                int st = dfa[i][j];
                for(int k=0;k<nfa_count[st][s];k++){
                    int dest = nfa[st][s][k];
                    int exists_flag=0;
                    for(int m=0;m<new_size;m++)
                        if(new_state[m]==dest){exists_flag=1; break;}
                    if(!exists_flag) new_state[new_size++] = dest;
                }
            }

            if(new_size>0){
                int idx = exists(new_state,new_size);
                if(idx==-1){
                    for(int j=0;j<new_size;j++) dfa[dfa_count][j] = new_state[j];
                    dfa_size[dfa_count] = new_size;
                    idx = dfa_count++;
                }
                dfa_trans[i][s] = idx;
            } else {
                dfa_trans[i][s] = -1; // no transition
            }
        }
    }

    // 5. Print DFA states
    printf("\nDFA States:\n");
    for(int i=0;i<dfa_count;i++){
        printf("D%d = { ", i);
        for(int j=0;j<dfa_size[i];j++)
            printf("q%d ", dfa[i][j]);
        printf("}\n");
    }

    // 6. Print DFA transitions
    printf("\nDFA Transitions:\n");
    for(int i=0;i<dfa_count;i++){
        for(int s=0;s<n_symbols;s++){
            if(dfa_trans[i][s]!=-1)
                printf("D%d --%c--> D%d\n", i, symbols[s], dfa_trans[i][s]);
        }
    }

    return 0;
}
