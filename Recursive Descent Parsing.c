#include <stdio.h>
#include <string.h>
#include <ctype.h>

char input[100];
int pos;

// Function prototypes
int E();
int EP();
int T();
int TP();
int F();

int main() {
    printf("Recursive Descent Parsing for the grammar:\n");
    printf("E  -> TE'\n");
    printf("E' -> +TE' | ε\n");
    printf("T  -> FT'\n");
    printf("T' -> *FT' | ε\n");
    printf("F  -> (E) | ID\n\n");

    printf("Enter the string to check (end with $): ");
    scanf("%s", input);
    
    // Initialize position
    pos = 0;
    
    if (E() && input[pos] == '$') {
        printf("\nString is accepted\n");
    } else {
        printf("\nString is not accepted\n");
    }
    
    return 0;
}

int E() {
    if (T()) {
        if (EP()) {
            return 1;
        }
    }
    return 0;
}

int EP() {
    if (input[pos] == '+') {
        pos++;
        if (T()) {
            if (EP()) {
                return 1;
            }
            return 0;
        }
        return 0;
    }
    // ε production (empty string is valid)
    return 1;
}

int T() {
    if (F()) {
        if (TP()) {
            return 1;
        }
    }
    return 0;
}

int TP() {
    if (input[pos] == '*') {
        pos++;
        if (F()) {
            if (TP()) {
                return 1;
            }
            return 0;
        }
        return 0;
    }
    // ε production (empty string is valid)
    return 1;
}

int F() {
    if (input[pos] == '(') {
        pos++;
        if (E()) {
            if (input[pos] == ')') {
                pos++;
                return 1;
            }
            return 0;
        }
        return 0;
    } else if (isalpha(input[pos])) {
        pos++;
        return 1;
    }
    return 0;
}