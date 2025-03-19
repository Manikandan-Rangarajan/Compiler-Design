#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX 100

char production[MAX][MAX], calc_first[MAX][MAX], calc_follow[MAX][MAX];
char first[MAX], f[MAX], stack[MAX], input[MAX];
int count, n, m, ptr;

void findfirst(char c, int q1, int q2);
void follow(char c);
void followfirst(char c, int c1, int c2);

void parse()
{
    char done[MAX] = {0}, ter[MAX] = {0};
    int i, j, k, sid = 0, land = 0;

    // Initialize arrays
    memset(calc_first, '!', sizeof(calc_first));
    memset(calc_follow, '!', sizeof(calc_follow));

    // Calculate FIRST sets
    ptr = -1;
    for (i = 0; i < count; i++)
    {
        if (!strchr(done, production[i][0]))
        {
            done[++ptr] = production[i][0];
            n = 0;
            findfirst(production[i][0], -1, -1);
            calc_first[ptr][0] = production[i][0];
            for (j = 0; j < n; j++)
                calc_first[ptr][j + 1] = first[j];
            land++;
        }
    }

    // Display FIRST sets
    printf("\nFIRST Sets:\n");
    for (i = 0; i < land; i++)
    {
        printf("FIRST(%c) = { ", calc_first[i][0]);
        for (j = 1; calc_first[i][j] != '!'; j++)
        {
            if (calc_first[i][j] == '#')
                printf("ε ");
            else
                printf("%c ", calc_first[i][j]);
        }
        printf("}\n");
    }

    // Calculate FOLLOW sets
    ptr = -1;
    memset(done, 0, sizeof(done));
    for (i = 0; i < count; i++)
    {
        if (!strchr(done, production[i][0]))
        {
            done[++ptr] = production[i][0];
            m = 0;
            follow(production[i][0]);
            calc_follow[ptr][0] = production[i][0];
            for (j = 0; j < m; j++)
                calc_follow[ptr][j + 1] = f[j];
        }
    }

    // Display FOLLOW sets
    printf("\nFOLLOW Sets:\n");
    for (i = 0; i <= ptr; i++)
    {
        printf("FOLLOW(%c) = { ", calc_follow[i][0]);
        for (j = 1; calc_follow[i][j] != '!'; j++)
        {
            if (calc_follow[i][j] == '#')
                printf("ε ");
            else
                printf("%c ", calc_follow[i][j]);
        }
        printf("}\n");
    }

    // Get terminals
    for (i = 0; i < count; i++)
        for (j = 2; production[i][j]; j++)
            if (!isupper(production[i][j]) && production[i][j] != '#' &&
                production[i][j] != '=' && !strchr(ter, production[i][j]))
                ter[sid++] = production[i][j];
    ter[sid++] = '$';
    ter[sid] = '\0';

    // Build parsing table
    char table[MAX][MAX];
    memset(table, '!', sizeof(table));
    for (i = 0; i < ptr + 1; i++)
        table[i][0] = calc_follow[i][0]; // Use the same order as done for consistency

    for (i = 0; i < count; i++)
    {
        char nt = production[i][0];
        int row = -1;
        // Find the row number for this non-terminal
        for (j = 0; j <= ptr; j++)
        {
            if (table[j][0] == nt)
            {
                row = j;
                break;
            }
        }
        if (row == -1)
            continue; // Skip if not found

        // If the production is of the form A -> ε
        if (production[i][2] == '#')
        {
            // For all terminals in FOLLOW(A)
            for (j = 1; calc_follow[row][j] != '!'; j++)
            {
                char terminal = calc_follow[row][j];
                // Find the column for this terminal
                for (k = 0; k < sid; k++)
                {
                    if (ter[k] == terminal)
                    {
                        table[row][k + 1] = i + '0'; // Use index of production
                        break;
                    }
                }
            }
        }
        else
        {
            n = 0;
            findfirst(production[i][2], -1, -1);
            for (j = 0; j < n; j++)
            {
                if (first[j] == '#')
                {
                    // For all terminals in FOLLOW(A)
                    for (int t = 1; calc_follow[row][t] != '!'; t++)
                    {
                        char terminal = calc_follow[row][t];
                        // Find the column for this terminal
                        for (k = 0; k < sid; k++)
                        {
                            if (ter[k] == terminal)
                            {
                                table[row][k + 1] = i + '0'; // Use index of production
                                break;
                            }
                        }
                    }
                }
                else
                {
                    char terminal = first[j];
                    // Find the column for this terminal
                    for (k = 0; k < sid; k++)
                    {
                        if (ter[k] == terminal)
                        {
                            table[row][k + 1] = i + '0'; // Use index of production
                            break;
                        }
                    }
                }
            }
        }
    }

    // Print table
    printf("\nLL(1) Parsing Table:\n\t");
    for (i = 0; i < sid; i++)
        printf("%c\t", ter[i]);
    printf("\n");
    for (i = 0; i <= ptr; i++)
    {
        printf("%c\t", table[i][0]);
        for (j = 1; j <= sid; j++)
        {
            if (table[i][j] == '!')
                printf("-\t");
            else
            {
                int prod_idx = table[i][j] - '0';
                printf("%s\t", production[prod_idx]);
            }
        }
        printf("\n");
    }

    // Parse input
    printf("\nEnter input string (end with $): ");
    scanf("%s", input);

    int i_ptr = 0, s_ptr = 1;
    stack[0] = '$';
    stack[1] = table[0][0]; // Start symbol
    stack[2] = '\0';

    printf("\nStack\t\tInput\t\tAction\n");
    while (s_ptr > 0)
    {
        for (i = 0; i <= s_ptr; i++)
            printf("%c", stack[i]);
        printf("\t\t");
        for (i = i_ptr; input[i] != '\0'; i++)
            printf("%c", input[i]);
        printf("\t\t");

        if (stack[s_ptr] == input[i_ptr])
        {
            printf("Match %c\n", stack[s_ptr]);
            s_ptr--;
            i_ptr++;
            if (stack[s_ptr] == '$' && input[i_ptr] == '$')
            {
                printf("$\t\t$\t\tAccept\n");
                printf("ACCEPTED\n");
                return;
            }
        }
        else if (!isupper(stack[s_ptr]))
        {
            printf("Error: Expected %c but got %c\n", stack[s_ptr], input[i_ptr]);
            printf("REJECTED\n");
            return;
        }
        else
        {
            // Current non-terminal in stack
            char nt = stack[s_ptr];
            // Current terminal in input
            char t = input[i_ptr];

            // Find row for non-terminal
            int row = -1;
            for (i = 0; i <= ptr; i++)
            {
                if (table[i][0] == nt)
                {
                    row = i;
                    break;
                }
            }

            // Find column for terminal
            int col = -1;
            for (i = 0; i < sid; i++)
            {
                if (ter[i] == t)
                {
                    col = i + 1;
                    break;
                }
            }

            if (row == -1 || col == -1 || table[row][col] == '!')
            {
                printf("Error: No production for %c with input %c\n", nt, t);
                printf("REJECTED\n");
                return;
            }

            int prod_idx = table[row][col] - '0';
            printf("%s\n", production[prod_idx]);

            // Replace the non-terminal with its production
            s_ptr--; // Remove the non-terminal

            // If it's not ε, push the production in reverse order
            if (production[prod_idx][2] != '#')
            {
                int len = strlen(production[prod_idx]);
                for (i = len - 1; i >= 2; i--)
                {
                    stack[++s_ptr] = production[prod_idx][i];
                }
            }
            stack[s_ptr + 1] = '\0';
        }
    }

    if (input[i_ptr] == '$')
    {
        printf("ACCEPTED\n");
    }
    else
    {
        printf("REJECTED\n");
    }
}

void findfirst(char c, int q1, int q2)
{
    int i, j;

    // If c is a terminal or ε
    if (!isupper(c))
    {
        first[n++] = c;
        return;
    }

    // If c is a non-terminal
    for (i = 0; i < count; i++)
    {
        if (production[i][0] == c)
        {
            // If production is of the form X -> ε
            if (production[i][2] == '#')
            {
                if (q1 == -1)
                {
                    first[n++] = '#';
                }
                else if (production[q1][q2] == '\0')
                {
                    // End of a production
                    first[n++] = '#';
                }
                else
                {
                    findfirst(production[q1][q2], q1, q2 + 1);
                }
            }
            else if (isupper(production[i][2]))
            {
                // If the first symbol is non-terminal
                findfirst(production[i][2], i, 3);
            }
            else
            {
                // If the first symbol is terminal
                first[n++] = production[i][2];
            }
        }
    }
}

void follow(char c)
{
    int i, j;

    // For the start symbol, add $ to follow
    if (c == production[0][0])
    {
        f[m++] = '$';
    }

    for (i = 0; i < count; i++)
    {
        for (j = 2; production[i][j] != '\0'; j++)
        {
            if (production[i][j] == c)
            {
                if (production[i][j + 1] != '\0')
                {
                    // If there is something after c
                    followfirst(production[i][j + 1], i, j + 2);
                }
                else
                {
                    // If c is the last symbol of the production
                    if (production[i][0] != c)
                    {
                        follow(production[i][0]);
                    }
                }
            }
        }
    }
}

void followfirst(char c, int c1, int c2)
{
    int i, j, k;

    // If c is a terminal
    if (!isupper(c))
    {
        for (i = 0; i < m; i++)
        {
            if (f[i] == c)
                return;
        }
        f[m++] = c;
        return;
    }

    // If c is a non-terminal
    // Find FIRST(c) from the calculated FIRST sets
    for (i = 0; i < count; i++)
    {
        if (calc_first[i][0] == c)
            break;
    }

    // Add all non-ε symbols from FIRST(c) to FOLLOW
    for (j = 1; calc_first[i][j] != '!' && calc_first[i][j] != '\0'; j++)
    {
        if (calc_first[i][j] != '#')
        {
            int found = 0;
            for (k = 0; k < m; k++)
            {
                if (f[k] == calc_first[i][j])
                {
                    found = 1;
                    break;
                }
            }
            if (!found)
            {
                f[m++] = calc_first[i][j];
            }
        }
    }

    // If ε is in FIRST(c), follow the next symbol or the head of the production
    for (j = 1; calc_first[i][j] != '!' && calc_first[i][j] != '\0'; j++)
    {
        if (calc_first[i][j] == '#')
        {
            if (production[c1][c2] == '\0')
            {
                follow(production[c1][0]);
            }
            else
            {
                followfirst(production[c1][c2], c1, c2 + 1);
            }
            break;
        }
    }
}

int main()
{
    printf("Enter number of productions: ");
    scanf("%d", &count);
    printf("Enter productions (e.g., A=aB):\n");
    for (int i = 0; i < count; i++)
        scanf("%s", production[i]);
    parse();
    return 0;
}