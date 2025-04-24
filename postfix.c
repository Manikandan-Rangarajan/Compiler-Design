#include <stdio.h>
#include <ctype.h>  // For isdigit()
#include <stdlib.h> // For exit()

int stack[20];
int top = -1;

void push(int x) {
    stack[++top] = x;
}

int pop() {
    if (top == -1) {
        printf("Stack underflow!\n");
        exit(1);
    }
    return stack[top--];
}

int main() {
    char exp[20];
    char *e;
    int n1, n2, n3, num;

    printf("Enter the postfix expression: ");
    scanf("%s", exp);
    e = exp;

    while (*e != '\0') {
        if (isdigit(*e)) {
            num = *e - '0'; // ASCII to int
            push(num);
        } else {
            n1 = pop();
            n2 = pop();
            switch (*e) {
                case '+': n3 = n2 + n1; break;
                case '-': n3 = n2 - n1; break;
                case '*': n3 = n2 * n1; break;
                case '/':
                    if (n1 == 0) {
                        printf("Division by zero!\n");
                        return 1;
                    }
                    n3 = n2 / n1;
                    break;
                default:
                    printf("Invalid operator: %c\n", *e);
                    return 1;
            }
            push(n3);
        }
        e++;
    }

    printf("\nThe result of expression %s = %d\n\n", exp, pop());
    return 0;
}
