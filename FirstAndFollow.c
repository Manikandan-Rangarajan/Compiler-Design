#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_PRODUCTIONS 20
#define MAX_SYMBOL_LENGTH 100
#define EPSILON '#'

// Structure to hold grammar production
typedef struct {
    char left;
    char right[10];
} Production;

// Function declarations
void calculateFirst(char symbol, int currentProd, int currentPos);
void calculateFollow(char symbol);
void calculateFollowFirst(char symbol, int prodIndex, int pos);
void initializeArrays(void);
void printFirst(char symbol, int index, int* uniqueCount);
void printFollow(char symbol, int index, int* uniqueCount);

// Global variables with meaningful names
Production grammar[MAX_PRODUCTIONS];
char firstSets[MAX_PRODUCTIONS][MAX_SYMBOL_LENGTH];
char followSets[MAX_PRODUCTIONS][MAX_SYMBOL_LENGTH];
char firstResult[MAX_SYMBOL_LENGTH];
char followResult[MAX_SYMBOL_LENGTH];
int numProductions = 0;
int firstCount = 0;
int followCount = 0;

int main() {
    // Initialize arrays
    initializeArrays();
    
    // Sample grammar
    numProductions = 8;
    grammar[0] = (Production){'E', "TR"};
    grammar[1] = (Production){'R', "+TR"};
    grammar[2] = (Production){'R', "#"};
    grammar[3] = (Production){'T', "FY"};
    grammar[4] = (Production){'Y', "*FY"};
    grammar[5] = (Production){'Y', "#"};
    grammar[6] = (Production){'F', "(E)"};
    grammar[7] = (Production){'F', "i"};

    // Calculate FIRST sets
    printf("\nFIRST Sets:\n");
    bool* processed = calloc(256, sizeof(bool));  // For tracking processed symbols
    int firstIndex = 0;
    
    for(int i = 0; i < numProductions; i++) {
        char symbol = grammar[i].left;
        if (!processed[symbol]) {
            calculateFirst(symbol, 0, 0);
            processed[symbol] = true;
            
            int uniqueCount = 0;
            printFirst(symbol, firstIndex++, &uniqueCount);
        }
    }

    // Calculate FOLLOW sets
    printf("\nFOLLOW Sets:\n");
    memset(processed, 0, 256 * sizeof(bool));  // Reset processed array
    int followIndex = 0;
    
    for(int i = 0; i < numProductions; i++) {
        char symbol = grammar[i].left;
        if (!processed[symbol]) {
            calculateFollow(symbol);
            processed[symbol] = true;
            
            int uniqueCount = 0;
            printFollow(symbol, followIndex++, &uniqueCount);
        }
    }

    free(processed);
    return 0;
}

void initializeArrays() {
    for(int i = 0; i < MAX_PRODUCTIONS; i++) {
        for(int j = 0; j < MAX_SYMBOL_LENGTH; j++) {
            firstSets[i][j] = '!';  // '!' marks end of set
            followSets[i][j] = '!';
        }
    }
}

void calculateFirst(char symbol, int currentProd, int currentPos) {
    if (!isupper(symbol)) {  // Terminal symbol
        firstResult[firstCount++] = symbol;
        return;
    }

    for(int i = 0; i < numProductions; i++) {
        if (grammar[i].left == symbol) {
            if (grammar[i].right[0] == EPSILON) {
                if (grammar[currentProd].right[currentPos] == '\0') {
                    firstResult[firstCount++] = EPSILON;
                }
                else if (currentProd != 0 || currentPos != 0) {
                    calculateFirst(grammar[currentProd].right[currentPos], 
                                 currentProd, currentPos + 1);
                }
                else {
                    firstResult[firstCount++] = EPSILON;
                }
            }
            else if (!isupper(grammar[i].right[0])) {
                firstResult[firstCount++] = grammar[i].right[0];
            }
            else {
                calculateFirst(grammar[i].right[0], i, 1);
            }
        }
    }
}

void calculateFollow(char symbol) {
    if (symbol == grammar[0].left) {  // Start symbol
        followResult[followCount++] = '$';
    }

    for(int i = 0; i < numProductions; i++) {
        for(int j = 0; grammar[i].right[j] != '\0'; j++) {
            if (grammar[i].right[j] == symbol) {
                if (grammar[i].right[j + 1] != '\0') {
                    calculateFollowFirst(grammar[i].right[j + 1], i, j + 2);
                }
                
                if (grammar[i].right[j + 1] == '\0' && symbol != grammar[i].left) {
                    calculateFollow(grammar[i].left);
                }
            }
        }
    }
}

void calculateFollowFirst(char symbol, int prodIndex, int pos) {
    if (!isupper(symbol)) {
        followResult[followCount++] = symbol;
        return;
    }

    for(int i = 0; i < numProductions; i++) {
        if (firstSets[i][0] == symbol) {
            int j = 1;
            while(firstSets[i][j] != '!') {
                if (firstSets[i][j] != EPSILON) {
                    followResult[followCount++] = firstSets[i][j];
                }
                else if (grammar[prodIndex].right[pos] == '\0') {
                    calculateFollow(grammar[prodIndex].left);
                }
                else {
                    calculateFollowFirst(grammar[prodIndex].right[pos], 
                                      prodIndex, pos + 1);
                }
                j++;
            }
            break;
        }
    }
}

void printFirst(char symbol, int index, int* uniqueCount) {
    printf("First(%c) = { ", symbol);
    firstSets[index][(*uniqueCount)++] = symbol;
    
    for(int i = 0; i < firstCount; i++) {
        bool isDuplicate = false;
        for(int j = 0; j < *uniqueCount; j++) {
            if (firstResult[i] == firstSets[index][j]) {
                isDuplicate = true;
                break;
            }
        }
        if (!isDuplicate) {
            printf("%c ", firstResult[i]);
            firstSets[index][(*uniqueCount)++] = firstResult[i];
        }
    }
    printf("}\n");
    firstCount = 0;  // Reset for next symbol
}

void printFollow(char symbol, int index, int* uniqueCount) {
    printf("Follow(%c) = { ", symbol);
    followSets[index][(*uniqueCount)++] = symbol;
    
    for(int i = 0; i < followCount; i++) {
        bool isDuplicate = false;
        for(int j = 0; j < *uniqueCount; j++) {
            if (followResult[i] == followSets[index][j]) {
                isDuplicate = true;
                break;
            }
        }
        if (!isDuplicate) {
            printf("%c ", followResult[i]);
            followSets[index][(*uniqueCount)++] = followResult[i];
        }
    }
    printf("}\n");
    followCount = 0;  // Reset for next symbol
}