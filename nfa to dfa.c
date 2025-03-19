// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <stdbool.h>

// #define MAX_STATES 100
// #define MAX_ALPHABET 26

// typedef struct {
//     int num_states;
//     int num_final_states;
//     int final_states[MAX_STATES];
//     int initial_state;
//     int transitions[MAX_STATES][MAX_ALPHABET][MAX_STATES];
//     int transition_counts[MAX_STATES][MAX_ALPHABET];
// } Automaton;

// void initialize_automaton(Automaton *a) {
//     memset(a->transitions, -1, sizeof(a->transitions));
//     memset(a->transition_counts, 0, sizeof(a->transition_counts));
// }

// void add_transition(Automaton *a, int from_state, char input, int to_state) {
//     int symbol = input - 'a';
//     a->transitions[from_state][symbol][a->transition_counts[from_state][symbol]++] = to_state;
// }

// bool is_final_state(Automaton *a, int state) {
//     for (int i = 0; i < a->num_final_states; i++) {
//         if (a->final_states[i] == state) 
//             return true;
//     }
//     return false;
// }

// bool process_string(Automaton *a, char *str) {
//     int current_states[MAX_STATES] = {a->initial_state};
//     int current_state_count = 1;
    
//     printf("String takes the following path--> %d", a->initial_state);
    
//     for (int i = 0; str[i] != '\0'; i++) {
//         int symbol = str[i] - 'a';
//         int next_states[MAX_STATES];
//         int next_state_count = 0;
        
//         // Explore all possible transitions for current states
//         for (int j = 0; j < current_state_count; j++) {
//             int current_state = current_states[j];
            
//             // Check if there are any transitions for this symbol from current state
//             if (a->transition_counts[current_state][symbol] > 0) {
//                 for (int k = 0; k < a->transition_counts[current_state][symbol]; k++) {
//                     int next_state = a->transitions[current_state][symbol][k];
                    
//                     // Add unique next states
//                     bool exists = false;
//                     for (int m = 0; m < next_state_count; m++) {
//                         if (next_states[m] == next_state) {
//                             exists = true;
//                             break;
//                         }
//                     }
                    
//                     if (!exists) {
//                         next_states[next_state_count++] = next_state;
//                         printf("-%c-%d", str[i], next_state);
//                     }
//                 }
//             }
//         }
        
//         // Update current states
//         memcpy(current_states, next_states, sizeof(int) * next_state_count);
//         current_state_count = next_state_count;
        
//         // If no states left, string is rejected
//         if (current_state_count == 0) {
//             printf("\nFinal state - Rejected");
//             return false;
//         }
//     }
    
//     // Check if any final states are reached
//     for (int i = 0; i < current_state_count; i++) {
//         if (is_final_state(a, current_states[i])) {
//             printf("\nFinal state - %d\nString Accepted", current_states[i]);
//             return true;
//         }
//     }
    
//     printf("\nFinal state - Rejected");
//     return false;
// }

// int main() {
//     Automaton nfa;
//     initialize_automaton(&nfa);
    
//     printf("Enter the number of states: ");
//     scanf("%d", &nfa.num_states);
    
//     printf("Enter number of final states: ");
//     scanf("%d", &nfa.num_final_states);
    
//     printf("Enter final states: ");
//     for (int i = 0; i < nfa.num_final_states; i++) {
//         scanf("%d", &nfa.final_states[i]);
//     }
    
//     printf("Enter the number of transition rules: ");
//     int num_rules;
//     scanf("%d", &num_rules);
    
//     printf("Define transition rules (initial_state input_symbol final_state):\n");
//     for (int i = 0; i < num_rules; i++) {
//         int from_state, to_state;
//         char input;
//         scanf("%d %c %d", &from_state, &input, &to_state);
//         add_transition(&nfa, from_state, input, to_state);
//     }
    
//     printf("Enter initial state: ");
//     scanf("%d", &nfa.initial_state);
    
//     char input_string[100];
//     while (1) {
//         printf("\nEnter string (or 'exit' to quit): ");
//         scanf("%s", input_string);
        
//         if (strcmp(input_string, "exit") == 0) 
//             break;
        
//         process_string(&nfa, input_string);
//     }
    
//     return 0;
// }

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_STATES 100
#define MAX_SYMBOLS 10
#define MAX_STATE_LENGTH 50

typedef struct {
    char states[MAX_STATE_LENGTH];
    bool is_processed;
} DFAState;

typedef struct {
    char nfa_table[MAX_STATES][MAX_SYMBOLS + 1][MAX_STATE_LENGTH];
    int num_states;
    int num_symbols;
} NFATransitionTable;

typedef struct {
    char dfa_table[MAX_STATES][MAX_SYMBOLS][MAX_STATE_LENGTH];
    DFAState dfa_states[MAX_STATES];
    int dfa_state_count;
} DFATransitionTable;

void reset_array(int *arr, int size) {
    memset(arr, 0, size * sizeof(int));
}

void compute_epsilon_closure(NFATransitionTable *nfa, int state, bool *closure, int *closure_arr) {
    reset_array(closure_arr, nfa->num_states);
    closure_arr[state] = 2;
    closure[state] = true;

    if (strcmp(nfa->nfa_table[state][nfa->num_symbols], "-") != 0) {
        for (int i = 0; i < nfa->num_states; i++) {
            if (strchr(nfa->nfa_table[state][nfa->num_symbols], 'A' + i) != NULL) {
                if (!closure[i]) {
                    compute_epsilon_closure(nfa, i, closure, closure_arr);
                }
            }
        }
    }
}

void generate_dfa_transition_table(NFATransitionTable *nfa, DFATransitionTable *dfa) {
    dfa->dfa_state_count = 0;
    
    // Initialize first DFA state with epsilon closure of start state
    bool initial_closure[MAX_STATES] = {false};
    int initial_closure_arr[MAX_STATES];
    compute_epsilon_closure(nfa, 0, initial_closure, initial_closure_arr);
    
    // Convert initial closure to state representation
    char initial_state[MAX_STATE_LENGTH] = {0};
    int idx = 0;
    for (int i = 0; i < nfa->num_states; i++) {
        if (initial_closure[i]) {
            initial_state[idx++] = 'A' + i;
        }
    }
    initial_state[idx] = '\0';
    
    // Add initial state to DFA states
    strcpy(dfa->dfa_states[dfa->dfa_state_count].states, initial_state);
    dfa->dfa_states[dfa->dfa_state_count].is_processed = false;
    dfa->dfa_state_count++;

    // Process DFA states
    for (int i = 0; i < dfa->dfa_state_count; i++) {
        if (!dfa->dfa_states[i].is_processed) {
            dfa->dfa_states[i].is_processed = true;

            // Compute transitions for each symbol
            for (int symbol = 0; symbol < nfa->num_symbols; symbol++) {
                bool transition_closure[MAX_STATES] = {false};
                int transition_closure_arr[MAX_STATES];
                reset_array(transition_closure_arr, nfa->num_states);

                // Compute transition for each state in current DFA state
                for (int j = 0; dfa->dfa_states[i].states[j]; j++) {
                    int nfa_state = dfa->dfa_states[i].states[j] - 'A';
                    
                    // Get NFA transitions for this state and symbol
                    if (strcmp(nfa->nfa_table[nfa_state][symbol], "-") != 0) {
                        for (int k = 0; nfa->nfa_table[nfa_state][symbol][k]; k++) {
                            int target_state = nfa->nfa_table[nfa_state][symbol][k] - 'A';
                            bool state_closure[MAX_STATES] = {false};
                            int state_closure_arr[MAX_STATES];
                            
                            compute_epsilon_closure(nfa, target_state, state_closure, state_closure_arr);
                            
                            // Merge closures
                            for (int m = 0; m < nfa->num_states; m++) {
                                if (state_closure[m]) {
                                    transition_closure[m] = true;
                                }
                            }
                        }
                    }
                }

                // Convert transition closure to state representation
                char transition_state[MAX_STATE_LENGTH] = {0};
                int state_idx = 0;
                for (int m = 0; m < nfa->num_states; m++) {
                    if (transition_closure[m]) {
                        transition_state[state_idx++] = 'A' + m;
                    }
                }
                transition_state[state_idx] = '\0';

                // If transition state is not empty, add to DFA
                if (transition_state[0] != '\0') {
                    // Check if this state already exists
                    bool state_exists = false;
                    for (int j = 0; j < dfa->dfa_state_count; j++) {
                        if (strcmp(dfa->dfa_states[j].states, transition_state) == 0) {
                            strcpy(dfa->dfa_table[i][symbol], transition_state);
                            state_exists = true;
                            break;
                        }
                    }

                    // If state doesn't exist, add it
                    if (!state_exists) {
                        strcpy(dfa->dfa_states[dfa->dfa_state_count].states, transition_state);
                        dfa->dfa_states[dfa->dfa_state_count].is_processed = false;
                        strcpy(dfa->dfa_table[i][symbol], transition_state);
                        dfa->dfa_state_count++;
                    }
                } else {
                    strcpy(dfa->dfa_table[i][symbol], "-");
                }
            }
        }
    }
}

void print_nfa_table(NFATransitionTable *nfa) {
    printf("\nNFA STATE TRANSITION TABLE\n");
    printf("STATES\t");
    for (int i = 0; i < nfa->num_symbols; i++)
        printf("|%d\t", i);
    printf("eps\n");
    printf("--------+------------------------------------\n");
    
    for (int i = 0; i < nfa->num_states; i++) {
        printf("%c\t", 'A' + i);
        for (int j = 0; j <= nfa->num_symbols; j++) {
            printf("|%s\t", nfa->nfa_table[i][j]);
        }
        printf("\n");
    }
}

void print_dfa_table(DFATransitionTable *dfa, int num_symbols) {
    printf("\n\nDFA TRANSITION STATE TABLE\n");
    printf("STATES\t");
    for (int i = 0; i < num_symbols; i++)
        printf("|%d\t", i);
    printf("\n");
    printf("--------+-----------------------\n");
    
    for (int i = 0; i < dfa->dfa_state_count; i++) {
        printf("%s\t", dfa->dfa_states[i].states);
        for (int j = 0; j < num_symbols; j++) {
            printf("|%s\t", dfa->dfa_table[i][j]);
        }
        printf("\n");
    }
}

int main() {
    NFATransitionTable nfa = {
        .num_states = 6,
        .num_symbols = 2,
        .nfa_table = {
            {"FC", "-", "BF"},
            {"-", "C", "-"},
            {"-", "-", "D"},
            {"E", "A", "-"},
            {"A", "-", "BF"},
            {"-", "-", "-"}
        }
    };

    DFATransitionTable dfa = {0};

    print_nfa_table(&nfa);
    generate_dfa_transition_table(&nfa, &dfa);
    print_dfa_table(&dfa, nfa.num_symbols);

    return 0;
}