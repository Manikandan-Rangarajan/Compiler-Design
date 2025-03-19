#include <stdio.h>
#include <string.h>
#include <ctype.h>

void main() {
    char regex[20], transitionTable[10][10];
    int regexLength, i, j, row = 0, charCounter = 0;

    printf("\n\t\t\tSIMULATION OF NFA");

    // Initialize the transition table with blank spaces
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            transitionTable[i][j] = ' ';
        }
    }

    printf("\nEnter a regular expression: ");
    scanf("%s", regex);
    regexLength = strlen(regex);

    for (i = 0; i < regexLength; i++) {
        switch (regex[i]) {
            case '|': {
                // Handling alternation (a|b)
                transitionTable[row][row + 1] = 'E'; // ε-transition to first branch
                transitionTable[row + 1][row + 2] = regex[i - 1]; // Transition for first operand
                transitionTable[row + 2][row + 5] = 'E'; // ε-transition to final state

                transitionTable[row][row + 3] = 'E'; // ε-transition to second branch
                transitionTable[row + 3][row + 4] = regex[i + 1]; // Transition for second operand
                transitionTable[row + 4][row + 5] = 'E'; // ε-transition to final state
                row = row + 5;
                break;
            }
            case '*': {
                // Handling Kleene Star (a*)
                transitionTable[row - 1][row] = 'E'; // ε-transition from previous state
                transitionTable[row][row + 1] = 'E'; // ε-transition to skip loop
                transitionTable[row][row + 3] = 'E'; // ε-transition to loop back
                transitionTable[row + 1][row + 2] = regex[i - 1]; // Transition for operand
                transitionTable[row + 2][row + 1] = 'E'; // ε-transition to repeat
                transitionTable[row + 2][row + 3] = 'E'; // ε-transition to final state
                row = row + 3;
                break;
            }
            case '+': {
                // Handling Plus operator (a+)
                transitionTable[row][row + 1] = regex[i - 1]; // Transition for operand
                transitionTable[row + 1][row] = 'E'; // ε-transition to repeat
                row = row + 1;
                break;
            }
            default: {
                // Handling regular characters
                if (charCounter == 0) {
                    if (isalpha(regex[i]) && isalpha(regex[i + 1])) {
                        transitionTable[row][row + 1] = regex[i];
                        transitionTable[row + 1][row + 2] = regex[i + 1];
                        row = row + 2;
                        charCounter = 1;
                    }
                } else if (charCounter == 1) {
                    if (isalpha(regex[i + 1])) {
                        transitionTable[row][row + 1] = regex[i + 1];
                        row = row + 1;
                        charCounter = 2;
                    }
                } else {
                    if (isalpha(regex[i + 1])) {
                        transitionTable[row][row + 1] = regex[i + 1];
                        row = row + 1;
                        charCounter = 3;
                    }
                }
                break;
            }
        }
    }

    // Print the transition table
    printf("\n");
    for (j = 0; j <= row; j++) {
        printf("  %d", j);
    }
    printf("\n___________________________________\n\n");
    for (i = 0; i <= row; i++) {
        for (j = 0; j <= row; j++) {
            if(transitionTable[i][j]!=' '){
                printf("  %c", transitionTable[i][j]);
            }else{
                printf("  -");
            }
        }
        printf("  | %d", i);
        
        printf("\n");
    }

    printf("\nStart state: 0\nFinal state: %d", row);
}