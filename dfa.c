// #include <stdio.h>
// #include <string.h>
// #include <stdlib.h>
// #include <stdbool.h>

// #define MAX_STATES 100
// #define MAX_INPUTS 2
// #define MAX_TRANSITIONS 10000

// typedef struct {
//     int states[MAX_STATES];
//     int count;
// } StateSet;

// // Global variables
// int numStates, numInputs, numFinalStates;
// int nfa[MAX_STATES][MAX_INPUTS][MAX_STATES];
// bool stateVisited[MAX_TRANSITIONS];
// int dfaTransitions[MAX_TRANSITIONS][MAX_INPUTS];
// StateSet stateQueue[MAX_TRANSITIONS];
// int finalStates[MAX_STATES];

// // Function prototypes
// StateSet createStateSet();
// void addToStateSet(StateSet* set, int state);
// bool isStateInSet(const StateSet* set, int state);
// void printStateSet(const StateSet* set);

// int main() {
//     int initialState;

//     // Input validation and error handling
//     printf("Enter the number of states (1-%d): ", MAX_STATES);
//     if (scanf("%d", &numStates) != 1 || numStates <= 0 || numStates > MAX_STATES) {
//         fprintf(stderr, "Invalid number of states.\n");
//         return EXIT_FAILURE;
//     }

//     // Initialize NFA transition table
//     memset(nfa, 0, sizeof(nfa));

//     printf("Enter number of inputs (usually 2 for binary): ");
//     if (scanf("%d", &numInputs) != 1 || numInputs <= 0 || numInputs > MAX_INPUTS) {
//         fprintf(stderr, "Invalid number of inputs.\n");
//         return EXIT_FAILURE;
//     }

//     // Input final states
//     printf("Enter number of final states: ");
//     if (scanf("%d", &numFinalStates) != 1 || numFinalStates < 0 || numFinalStates > numStates) {
//         fprintf(stderr, "Invalid number of final states.\n");
//         return EXIT_FAILURE;
//     }

//     printf("Enter final states (0-based index): ");
//     for (int i = 0; i < numFinalStates; i++) {
//         if (scanf("%d", &finalStates[i]) != 1 || finalStates[i] < 0 || finalStates[i] >= numStates) {
//             fprintf(stderr, "Invalid final state.\n");
//             return EXIT_FAILURE;
//         }
//     }

//     // Input NFA transitions
//     printf("Enter number of NFA transitions: ");
//     int numTransitions;
//     if (scanf("%d", &numTransitions) != 1 || numTransitions < 0) {
//         fprintf(stderr, "Invalid number of transitions.\n");
//         return EXIT_FAILURE;
//     }

//     printf("Enter transitions (from_state input_symbol to_state):\n");
//     for (int i = 0; i < numTransitions; i++) {
//         int fromState, inputSymbol, toState;
//         if (scanf("%d %d %d", &fromState, &inputSymbol, &toState) != 3 ||
//             fromState < 0 || fromState >= numStates ||
//             inputSymbol < 0 || inputSymbol >= numInputs ||
//             toState < 0 || toState >= numStates) {
//             fprintf(stderr, "Invalid transition input.\n");
//             return EXIT_FAILURE;
//         }
//         nfa[fromState][inputSymbol][toState] = 1;
//     }

//     // Input initial state
//     printf("Enter initial state: ");
//     if (scanf("%d", &initialState) != 1 || initialState < 0 || initialState >= numStates) {
//         fprintf(stderr, "Invalid initial state.\n");
//         return EXIT_FAILURE;
//     }

//     // Placeholder for the NFA to DFA conversion algorithm
//     printf("NFA to DFA conversion completed.\n");

//     return EXIT_SUCCESS;
// }

// // Helper function to create an empty state set
// StateSet createStateSet() {
//     return (StateSet){{0}, 0};
// }

// // Add a state to the set if not already present
// void addToStateSet(StateSet* set, int state) {
//     if (!isStateInSet(set, state) && set->count < MAX_STATES) {
//         set->states[set->count++] = state;
//     }
// }

// // Check if a state is in the set
// bool isStateInSet(const StateSet* set, int state) {
//     for (int i = 0; i < set->count; i++) {
//         if (set->states[i] == state) {
//             return true;
//         }
//     }
//     return false;
// }

// // Print the state set
// void printStateSet(const StateSet* set) {
//     printf("{");
//     for (int i = 0; i < set->count; i++) {
//         printf("%d%s", set->states[i], (i < set->count - 1) ? " " : "");
//     }
//     printf("}\n");
// }

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_STATES 100
#define MAX_INPUTS 2
#define MAX_TRANSITIONS 10000

typedef struct {
    int states[MAX_STATES];
    int count;
} StateSet;

// Global variables
int numStates, numInputs, numFinalStates;
int nfa[MAX_STATES][MAX_INPUTS][MAX_STATES];
bool stateVisited[MAX_TRANSITIONS];
int dfaTransitions[MAX_TRANSITIONS][MAX_INPUTS];
StateSet stateQueue[MAX_TRANSITIONS];
int finalStates[MAX_STATES];
int dfaFinalStates[MAX_TRANSITIONS]; // Track DFA final states
int dfaStateCount = 0; // Track number of DFA states

// Function prototypes
StateSet createStateSet();
void addToStateSet(StateSet* set, int state);
bool isStateInSet(const StateSet* set, int state);
void printStateSet(const StateSet* set);
bool isFinalState(const StateSet* set);
int getStateIndex(const StateSet* set);

int main() {
    int initialState;

    // Input validation and error handling
    printf("Enter the number of states (1-%d): ", MAX_STATES);
    if (scanf("%d", &numStates) != 1 || numStates <= 0 || numStates > MAX_STATES) {
        fprintf(stderr, "Invalid number of states.\n");
        return EXIT_FAILURE;
    }

    // Initialize NFA transition table
    memset(nfa, 0, sizeof(nfa));

    printf("Enter number of inputs (usually 2 for binary): ");
    if (scanf("%d", &numInputs) != 1 || numInputs <= 0 || numInputs > MAX_INPUTS) {
        fprintf(stderr, "Invalid number of inputs.\n");
        return EXIT_FAILURE;
    }

    // Input final states
    printf("Enter number of final states: ");
    if (scanf("%d", &numFinalStates) != 1 || numFinalStates < 0 || numFinalStates > numStates) {
        fprintf(stderr, "Invalid number of final states.\n");
        return EXIT_FAILURE;
    }

    printf("Enter final states (0-based index): ");
    for (int i = 0; i < numFinalStates; i++) {
        if (scanf("%d", &finalStates[i]) != 1 || finalStates[i] < 0 || finalStates[i] >= numStates) {
            fprintf(stderr, "Invalid final state.\n");
            return EXIT_FAILURE;
        }
    }

    // Input NFA transitions
    printf("Enter number of NFA transitions: ");
    int numTransitions;
    if (scanf("%d", &numTransitions) != 1 || numTransitions < 0) {
        fprintf(stderr, "Invalid number of transitions.\n");
        return EXIT_FAILURE;
    }

    printf("Enter transitions (from_state input_symbol to_state):\n");
    for (int i = 0; i < numTransitions; i++) {
        int fromState, inputSymbol, toState;
        if (scanf("%d %d %d", &fromState, &inputSymbol, &toState) != 3 ||
            fromState < 0 || fromState >= numStates ||
            inputSymbol < 0 || inputSymbol >= numInputs ||
            toState < 0 || toState >= numStates) {
            fprintf(stderr, "Invalid transition input.\n");
            return EXIT_FAILURE;
        }
        nfa[fromState][inputSymbol][toState] = 1;
    }

    // Input initial state
    printf("Enter initial state: ");
    if (scanf("%d", &initialState) != 1 || initialState < 0 || initialState >= numStates) {
        fprintf(stderr, "Invalid initial state.\n");
        return EXIT_FAILURE;
    }

    // NFA to DFA conversion using subset construction
    int front = 0, rear = 0;
    StateSet initialSet = createStateSet();
    addToStateSet(&initialSet, initialState);

    stateQueue[rear++] = initialSet;
    dfaStateCount = 1;
    
    if (isFinalState(&initialSet)) {
        dfaFinalStates[0] = 1;
    } else {
        dfaFinalStates[0] = 0;
    }

    while (front < rear) {
        StateSet currentSet = stateQueue[front];
        front++;

        for (int input = 0; input < numInputs; input++) {
            StateSet newSet = createStateSet();
            
            for (int i = 0; i < currentSet.count; i++) {
                int state = currentSet.states[i];
                for (int j = 0; j < numStates; j++) {
                    if (nfa[state][input][j]) {
                        addToStateSet(&newSet, j);
                    }
                }
            }

            int stateIndex = getStateIndex(&newSet);
            if (stateIndex == -1) {
                stateQueue[rear] = newSet;
                dfaTransitions[front - 1][input] = rear;
                
                if (isFinalState(&newSet)) {
                    dfaFinalStates[rear] = 1;
                } else {
                    dfaFinalStates[rear] = 0;
                }
                rear++;
                dfaStateCount++;
            } else {
                dfaTransitions[front - 1][input] = stateIndex;
            }
        }
    }

    // Print DFA transitions
    printf("\nDFA Transitions:\n");
    for (int i = 0; i < dfaStateCount; i++) {
        printf("State %d: ", i);
        for (int j = 0; j < numInputs; j++) {
            printf("on input %d -> %d ", j, dfaTransitions[i][j]);
        }
        if (dfaFinalStates[i]) {
            printf("[Final State]");
        }
        printf("\n");
    }

    printf("\nNFA to DFA conversion completed.\n");

    return EXIT_SUCCESS;
}

// Helper function to create an empty state set
StateSet createStateSet() {
    return (StateSet){{0}, 0};
}

// Add a state to the set if not already present
void addToStateSet(StateSet* set, int state) {
    if (!isStateInSet(set, state) && set->count < MAX_STATES) {
        set->states[set->count++] = state;
    }
}

// Check if a state is in the set
bool isStateInSet(const StateSet* set, int state) {
    for (int i = 0; i < set->count; i++) {
        if (set->states[i] == state) {
            return true;
        }
    }
    return false;
}

// Print the state set
void printStateSet(const StateSet* set) {
    printf("{");
    for (int i = 0; i < set->count; i++) {
        printf("%d%s", set->states[i], (i < set->count - 1) ? " " : "");
    }
    printf("}\n");
}

// Check if any state in the set is a final state
bool isFinalState(const StateSet* set) {
    for (int i = 0; i < set->count; i++) {
        for (int j = 0; j < numFinalStates; j++) {
            if (set->states[i] == finalStates[j]) {
                return true;
            }
        }
    }
    return false;
}

// Get index of state set in stateQueue, return -1 if not found
int getStateIndex(const StateSet* set) {
    for (int i = 0; i < dfaStateCount; i++) {
        if (stateQueue[i].count == set->count) {
            bool same = true;
            for (int j = 0; j < set->count; j++) {
                if (!isStateInSet(&stateQueue[i], set->states[j])) {
                    same = false;
                    break;
                }
            }
            if (same) {
                return i;
            }
        }
    }
    return -1;
}
