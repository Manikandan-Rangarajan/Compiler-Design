#include <stdio.h>
#include <string.h>

void main() {
    char input[100], l[50], r[50], temp[10], productions[25][50];
    int i = 0, j = 0, flag = 0, consumed = 0;

    printf("Enter the Production (Format: A->aA|b): ");
    scanf("%s", input);

    // Splitting Left and Right parts of the Production
    sscanf(input, "%[^->]->%s", l, r);
    
    while (sscanf(r + consumed, "%[^|]s", temp) == 1 && consumed < strlen(r)) {
        if (temp[0] == l[0]) {  // Checking for left recursion
            flag = 1;
            sprintf(productions[i++], "%s'->%s%s'", l, temp + 1, l);
        } else {
            sprintf(productions[i++], "%s->%s%s'", l, temp, l);
        }
        consumed += strlen(temp) + 1;
    }

    if (flag) {
        sprintf(productions[i++], "%s'->ε", l);
        printf("\nThe productions after eliminating Left Recursion are:\n");
        for (j = 0; j < i; j++)
            printf("%s\n", productions[j]);
    } else {
        printf("\nThe Given Grammar has no Left Recursion.\n");
    }
}
