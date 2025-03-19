#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global variables
char parsingStack[100], tempBuffer[10];
int stackPointer = -1;
char terminals[6] = {'i', '+', '*', ')', '(', '$'};
char nonTerminals[3] = {'E', 'T', 'F'};
char stateSymbols[12] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'm', 'j', 'k', 'l'};

// Action table - compressed format
const char *actionTable[12][6] = {
    {"sf", "emp", "emp", "se", "emp", "emp"}, {"emp", "sg", "emp", "emp", "emp", "acc"},
    {"emp", "rc", "sh", "emp", "rc", "rc"}, {"emp", "re", "re", "emp", "re", "re"},
    {"sf", "emp", "emp", "se", "emp", "emp"}, {"emp", "rg", "rg", "emp", "rg", "rg"},
    {"sf", "emp", "emp", "se", "emp", "emp"}, {"sf", "emp", "emp", "se", "emp", "emp"},
    {"emp", "sg", "emp", "emp", "sl", "emp"}, {"emp", "rb", "sh", "emp", "rb", "rb"},
    {"emp", "rb", "rd", "emp", "rd", "rd"}, {"emp", "rf", "rf", "emp", "rf", "rf"}
};

// Goto table - compressed format
const char *gotoTable[12][3] = {
    {"b", "c", "d"}, {"emp", "emp", "emp"}, {"emp", "emp", "emp"}, {"emp", "emp", "emp"},
    {"i", "c", "d"}, {"emp", "emp", "emp"}, {"emp", "j", "d"}, {"emp", "emp", "k"},
    {"emp", "emp", "emp"}, {"emp", "emp", "emp"}, {"emp", "emp", "emp"}, {"emp", "emp", "emp"}
};

// Grammar rules
struct { 
    char leftSide; 
    char rightSide[5]; 
} grammarRules[6] = {
    {'E', "e+T"}, {'E', "T"}, {'T', "T*F"}, {'T', "F"}, {'F', "(E)"}, {'F', "i"}
};

// Function declarations
void pushToStack(char item) { parsingStack[++stackPointer] = item; }
char popFromStack() { return parsingStack[stackPointer--]; }
char getStackTop() { return parsingStack[stackPointer]; }

// Find indices in tables
int findTerminalIndex(char symbol) { 
    for(int i = 0; i < 6; i++) 
        if(symbol == terminals[i]) 
            return i + 1; 
    return 0; 
}

int findNonTerminalIndex(char symbol) { 
    for(int i = 0; i < 3; i++) 
        if(symbol == nonTerminals[i]) 
            return i + 1; 
    return 0; 
}

int findStateIndex(char state) { 
    for(int i = 0; i < 12; i++) 
        if(state == stateSymbols[i]) 
            return i + 1; 
    return 0; 
}

// Get production from tables
void getActionEntry(char inputSymbol, char currentState) {
    int symbolIndex = findTerminalIndex(inputSymbol);
    int stateIndex = findStateIndex(currentState);
    
    if(symbolIndex > 0 && stateIndex > 0)
        strcpy(tempBuffer, actionTable[stateIndex-1][symbolIndex-1]);
    else
        strcpy(tempBuffer, "emp");
}

void getGotoEntry(char state, char nonTerminal) {
    int stateIndex = findStateIndex(state);
    int nonTerminalIndex = findNonTerminalIndex(nonTerminal);
    
    if(stateIndex > 0 && nonTerminalIndex > 0)
        strcpy(tempBuffer, gotoTable[stateIndex-1][nonTerminalIndex-1]);
    else
        strcpy(tempBuffer, "emp");
}

// Print state in numeric form
void printNumericState(char stateChar) {
    switch(stateChar) {
        case 'a': printf("0"); break;
        case 'b': printf("1"); break;
        case 'c': printf("2"); break;
        case 'd': printf("3"); break;
        case 'e': printf("4"); break;
        case 'f': printf("5"); break;
        case 'g': printf("6"); break;
        case 'h': printf("7"); break;
        case 'm': printf("8"); break;
        case 'j': printf("9"); break;
        case 'k': printf("10"); break;
        case 'l': printf("11"); break;
        default: printf("%c", stateChar);
    }
}

// Print stack and input
void printParseStatus(char inputString[], int currentPos) {
    printf("\n");
    for(int r = 0; r <= stackPointer; r++) 
        printNumericState(parsingStack[r]);
    printf("\t\t\t");
    printf("%s", inputString + currentPos);
}

// Error handling
void reportError() { 
    printf(" error in the input "); 
    exit(0); 
}

int main() {
    char inputString[80], currentSymbol, currentState, leftSymbol[2] = {0}; 
    int currentPos = 0, ruleIndex, rightSideLength, popCount;
    
    printf(" Enter the input: ");
    scanf("%s", inputString);
    
    strcat(inputString, "$"); // Append end marker
    pushToStack('a'); // Initialize stack with start state
    
    printf("\n stack \t\t\t input");
    printParseStatus(inputString, currentPos);
    
    while(1) {
        currentSymbol = inputString[currentPos];
        currentState = getStackTop();
        getActionEntry(currentSymbol, currentState);
        
        if(strcmp(tempBuffer, "emp") == 0) 
            reportError();
            
        if(strcmp(tempBuffer, "acc") == 0) 
            break;
        
        if(tempBuffer[0] == 's') {
            // Shift operation
            pushToStack(inputString[currentPos++]);
            pushToStack(tempBuffer[1]);
        } else {
            // Reduce operation
            ruleIndex = findStateIndex(tempBuffer[1]);
            strcpy(tempBuffer, grammarRules[ruleIndex-2].rightSide);
            leftSymbol[0] = grammarRules[ruleIndex-2].leftSide;
            rightSideLength = strlen(tempBuffer);
            
            // Pop 2 * length of right side
            for(popCount = 0; popCount < 2 * rightSideLength; popCount++) 
                popFromStack();
            
            pushToStack(leftSymbol[0]);
            getGotoEntry(parsingStack[stackPointer-1], leftSymbol[0]);
            pushToStack(tempBuffer[0]);
        }
        
        printParseStatus(inputString, currentPos);
    }
    
    printf("\n %s the input", strcmp(tempBuffer, "acc") == 0 ? "accept" : "do not accept");
    return 0;
}