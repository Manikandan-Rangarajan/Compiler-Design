#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * Exits the program if the grammar is not an operator grammar
 */
void exitWithError() {
    printf("Not operator grammar\n");
    exit(0);
}

/**
 * Checks if the given character is a valid operator
 * @param op Character to check
 * @return 1 if valid operator, 0 otherwise
 */
int isValidOperator(char op) {
    return (op == '+' || op == '-' || op == '*' || op == '/');
}

int main() {
    char productions[20][20];
    int numProductions;
    int i, j;
    int isOperatorGrammar = 0;
    
    // Get number of productions
    printf("Enter number of productions: ");
    scanf("%d", &numProductions);
    
    // Get each production
    printf("Enter the productions:\n");
    for (i = 0; i < numProductions; i++) {
        scanf("%s", productions[i]);
    }
    
    // Validate each production
    for (i = 0; i < numProductions; i++) {
        // Reset position counter for each production
        j = 2;  // Start at index 2 (after LHS and '=')
        
        // Process each character in RHS
        while (productions[i][j] != '\0') {
            // Check for epsilon production
            if (productions[i][j] == '$') {
                exitWithError();
            }
            
            // Check if we're at position where an operator should be (index 3)
            if (j == 3 && isValidOperator(productions[i][j])) {
                isOperatorGrammar = 1;
            } 
            else if (j == 3 && !isValidOperator(productions[i][j])) {
                exitWithError();
            }
            
            j++;
        }
    }
    
    // If we reached here without exiting, check if we found valid operators
    if (isOperatorGrammar) {
        printf("Operator grammar\n");
    } else {
        printf("Not operator grammar\n");
    }
    
    return 0;
}