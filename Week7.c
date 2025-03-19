#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_INPUT_LENGTH 100
#define MAX_STACK_SIZE 50

// Global variables
char input[MAX_INPUT_LENGTH];
char stack[MAX_STACK_SIZE];
int stack_top = -1;

// Function prototypes
void initialize();
void push(char c);
char pop();
bool is_stack_empty();
void shift_reduce_parse();
bool reduce_rules();
void print_parse_step(const char* action);

int main() {
    // Grammar explanation
    printf("GRAMMAR:\n");
    printf("E -> E + E\n");
    printf("E -> E * E\n");
    printf("E -> (E)\n");
    printf("E -> id\n\n");

    // Input prompt
    printf("Enter input string: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';  // Remove newline

    // Print table header
    printf("Stack\t\tInput\t\tAction\n");
    printf("------------------------------------------\n");

    // Initialize and start parsing
    initialize();
    shift_reduce_parse();

    return 0;
}

void initialize() {
    memset(stack, 0, sizeof(stack));
    stack_top = -1;
}

void push(char c) {
    if (stack_top < MAX_STACK_SIZE - 1) {
        stack[++stack_top] = c;
    }
}

char pop() {
    if (!is_stack_empty()) {
        return stack[stack_top--];
    }
    return '\0';
}

bool is_stack_empty() {
    return stack_top == -1;
}

void shift_reduce_parse() {
    int input_index = 0;
    
    while (input[input_index] != '\0') {
        // Handle identifier (id)
        if (input[input_index] == 'i' && input[input_index + 1] == 'd') {
            push('i');
            push('d');
            input[input_index] = input[input_index + 1] = ' ';
            print_parse_step("SHIFT->id");
            input_index += 2;
        }
        // Handle other symbols
        else {
            push(input[input_index]);
            input[input_index] = ' ';
            print_parse_step("SHIFT->symbol");
            input_index++;
        }

        // Try to reduce after each shift
        while (reduce_rules()) {
            print_parse_step("REDUCE TO E");
        }
    }

    // Final reduction phase
    while (reduce_rules()) {
        print_parse_step("REDUCE TO E");
    }
}

bool reduce_rules() {
    int len = stack_top + 1;

    // Rule: id -> E
    if (len >= 2 && stack[stack_top-1] == 'i' && stack[stack_top] == 'd') {
        stack_top -= 1;
        stack[stack_top] = 'E';
        return true;
    }

    // Rule: E + E -> E
    if (len >= 3 && stack[stack_top-2] == 'E' && 
        stack[stack_top-1] == '+' && stack[stack_top] == 'E') {
        stack_top -= 2;
        stack[stack_top] = 'E';
        return true;
    }

    // Rule: E * E -> E
    if (len >= 3 && stack[stack_top-2] == 'E' && 
        stack[stack_top-1] == '*' && stack[stack_top] == 'E') {
        stack_top -= 2;
        stack[stack_top] = 'E';
        return true;
    }

    // Rule: (E) -> E
    if (len >= 3 && stack[stack_top-2] == '(' && 
        stack[stack_top-1] == 'E' && stack[stack_top] == ')') {
        stack_top -= 2;
        stack[stack_top] = 'E';
        return true;
    }

    return false;
}

void print_parse_step(const char* action) {
    // Create stack string
    char stack_str[MAX_STACK_SIZE + 1] = "$";
    strncat(stack_str, stack, stack_top + 1);

    // Create input string
    char input_str[MAX_INPUT_LENGTH + 2] = "$";
    strcat(input_str, input);

    printf("%-15s %-15s %s\n", stack_str, input_str, action);
}