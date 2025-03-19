#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_STATES 100
#define MAX_INPUTS 3  // Including epsilon transitions
#define EPSILON 2     // Representing epsilon as input 2
#define MAX_TRANSITIONS 10000

typedef struct {
    int states[MAX_STATES];
    int count;
} StateSet;

// Global variables
int numStates, numInputs, numFinalStates;
int nfa[MAX_STATES][MAX_INPUTS][MAX_STATES];
bool stateVisited[MAX_TRANSITIONS];
int dfaTransitions[MAX_TRANSITIONS][MAX_INPUTS - 1];  // Exclude epsilon from DFA transitions
StateSet stateQueue[MAX_TRANSITIONS];
int finalStates[MAX_STATES];
int dfaFinalStates[MAX_TRANSITIONS];
int dfaStateCount = 0;

// Function prototypes
StateSet createStateSet();
void addToStateSet(StateSet* set, int state);
bool isStateInSet(const StateSet* set, int state);
void printStateSet(const StateSet* set);
bool isFinalState(const StateSet* set);
int getStateIndex(const StateSet* set);
void epsilonClosure(StateSet* set);

int main() {
    int initialState;

    // Input validation and error handling
    printf("Enter the number of states (1-%d): ", MAX_STATES);
    if (scanf("%d", &numStates) != 1 || numStates <= 0 || numStates > MAX_STATES) {
        fprintf(stderr, "Invalid number of states.\n");
        return EXIT_FAILURE;
    }

    memset(nfa, 0, sizeof(nfa));

    printf("Enter number of inputs (excluding epsilon, usually 2 for binary): ");
    if (scanf("%d", &numInputs) != 1 || numInputs <= 0 || numInputs > MAX_INPUTS - 1) {
        fprintf(stderr, "Invalid number of inputs.\n");
        return EXIT_FAILURE;
    }
    numInputs++;  // Account for epsilon transitions

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

    printf("Enter number of NFA transitions: ");
    int numTransitions;
    if (scanf("%d", &numTransitions) != 1 || numTransitions < 0) {
        fprintf(stderr, "Invalid number of transitions.\n");
        return EXIT_FAILURE;
    }

    printf("Enter transitions (from_state input_symbol to_state). Use %d for epsilon:\n", EPSILON);
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

    printf("Enter initial state: ");
    if (scanf("%d", &initialState) != 1 || initialState < 0 || initialState >= numStates) {
        fprintf(stderr, "Invalid initial state.\n");
        return EXIT_FAILURE;
    }

    int front = 0, rear = 0;
    StateSet initialSet = createStateSet();
    addToStateSet(&initialSet, initialState);
    epsilonClosure(&initialSet);

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

        for (int input = 0; input < numInputs - 1; input++) {
            StateSet newSet = createStateSet();

            for (int i = 0; i < currentSet.count; i++) {
                int state = currentSet.states[i];
                for (int j = 0; j < numStates; j++) {
                    if (nfa[state][input][j]) {
                        addToStateSet(&newSet, j);
                    }
                }
            }

            epsilonClosure(&newSet);

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

    printf("\nDFA Transitions:\n");
    for (int i = 0; i < dfaStateCount; i++) {
        printf("State %d: ", i);
        for (int j = 0; j < numInputs - 1; j++) {
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

StateSet createStateSet() {
    return (StateSet){{0}, 0};
}

void addToStateSet(StateSet* set, int state) {
    if (!isStateInSet(set, state) && set->count < MAX_STATES) {
        set->states[set->count++] = state;
    }
}

bool isStateInSet(const StateSet* set, int state) {
    for (int i = 0; i < set->count; i++) {
        if (set->states[i] == state) {
            return true;
        }
    }
    return false;
}

void printStateSet(const StateSet* set) {
    printf("{");
    for (int i = 0; i < set->count; i++) {
        printf("%d%s", set->states[i], (i < set->count - 1) ? " " : "");
    }
    printf("}\n");
}

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

void epsilonClosure(StateSet* set) {
    for (int i = 0; i < set->count; i++) {
        int state = set->states[i];
        for (int j = 0; j < numStates; j++) {
            if (nfa[state][EPSILON][j] && !isStateInSet(set, j)) {
                addToStateSet(set, j);
            }
        }
    }

    printf("Epsilon closure: ");
    printStateSet(set);
}
