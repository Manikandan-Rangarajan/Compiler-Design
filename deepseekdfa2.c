#include <stdio.h>
#include <string.h>

int dfa[100][2][100] = {0}; // NFA transition table
int state[10000] = {0};     // To track DFA states
int go[10000][2] = {0};     // DFA transition table
int arr[10000] = {0};       // To store new DFA states

int main() {
    int st, fin, in;
    int f[10];              // Final states
    int i, j, k, l;
    int flag = 0, curr1;

    printf("\nFollow the one-based indexing\n");

    // Input number of states
    printf("\nEnter the number of states: ");
    scanf("%d", &st);

    // Initialize states
    for (i = 0; i < st; i++)
        state[1 << i] = 1;

    // Input number of final states
    printf("\nEnter number of final states: ");
    scanf("%d", &fin);

    // Input final states
    printf("\nEnter final states: ");
    for (i = 0; i < fin; i++) {
        scanf("%d", &f[i]);
    }

    // Input number of transition rules
    int rel;
    printf("\nEnter the number of transition rules: ");
    scanf("%d", &rel);

    // Input transition rules
    printf("\nDefine transition rules as \"initial state input symbol final state\"\n");
    for (i = 0; i < rel; i++) {
        int p, q, r;
        scanf("%d%d%d", &p, &q, &r);
        if (q == 0)
            dfa[p][0][r] = 1;
        else
            dfa[p][1][r] = 1;
    }

    // Input initial state
    printf("\nEnter initial state: ");
    scanf("%d", &in);
    in = 1 << in;

    // Construct DFA
    printf("\nSolving according to DFA\n");
    int x = 0;
    for (i = 0; i < st; i++) {
        for (j = 0; j < 2; j++) {
            int stf = 0;
            for (k = 0; k < st; k++) {
                if (dfa[i][j][k] == 1)
                    stf |= (1 << k);
            }
            go[1 << i][j] = stf;
            printf("%d-%d-->%d\n", 1 << i, j, stf);
            if (state[stf] == 0) {
                arr[x++] = stf;
                state[stf] = 1;
            }
        }
    }

    // Process new states
    for (i = 0; i < x; i++) {
        for (j = 0; j < 2; j++) {
            int new = 0;
            for (k = 0; k < st; k++) {
                if (arr[i] & (1 << k)) {
                    new |= go[1 << k][j];
                }
            }
            if (state[new] == 0) {
                arr[x++] = new;
                state[new] = 1;
            }
        }
    }

    // Print DFA states and transitions
    printf("\nThe total number of distinct states are:\n");
    printf("STATE     0   1\n");
    for (i = 0; i < 10000; i++) {
        if (state[i] == 1) {
            if (i == 0)
                printf("q0 ");
            else {
                for (j = 0; j < st; j++) {
                    if (i & (1 << j))
                        printf("q%d ", j);
                }
            }
            printf("       %d   %d\n", go[i][0], go[i][1]);
        }
    }

    // Test strings
    int testCases = 3;
    char str[1000];
    while (testCases--) {
        printf("\nEnter a string: ");
        scanf("%s", str);
        l = strlen(str);
        curr1 = in;
        flag = 0;
        printf("\nString takes the following path:\n");
        printf("%d-", curr1);

        for (i = 0; i < l; i++) {
            if (str[i] != '0' && str[i] != '1') {
                printf("\nInvalid input symbol '%c'. Only 0 and 1 are allowed.\n", str[i]);
                flag = -1;
                break;
            }
            curr1 = go[curr1][str[i] - '0'];
            printf("%d-", curr1);
        }

        if (flag != -1) {
            printf("\nFinal state: %d\n", curr1);
            for (i = 0; i < fin; i++) {
                if (curr1 & (1 << f[i])) {
                    flag = 1;
                    break;
                }
            }
            if (flag)
                printf("String Accepted\n");
            else
                printf("String Rejected\n");
        }
    }

    return 0;
}