#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>

#define MAX_PROD 20
#define MAX_LEN 20

void followfirst(char, int, int);
void findfirst(char, int, int);
void follow(char c);

int count, n = 0;
char calc_first[MAX_PROD][100];
char calc_follow[MAX_PROD][100];
int m = 0;
char production[MAX_PROD][MAX_LEN], first[100];
char f[100];
int k;
char ck;
int e;

int main()
{
    int jm = 0;
    int km = 0;
    int i;
    char c, ch;
    
    printf("How many productions? : ");
    scanf("%d", &count);
    getchar(); // Consume newline
    
    if(count <= 0 || count > MAX_PROD) {
        printf("Invalid number of productions. Please enter a value between 1 and %d.\n", MAX_PROD);
        return 1;
    }
    
    printf("\nEnter %d productions in form A=B where A and B are grammar symbols :\n\n", count);
    for(i = 0; i < count; i++)
    {
        printf("Production %d: ", i+1);
        fgets(production[i], MAX_LEN, stdin);
        
        // Remove trailing newline
        int len = strlen(production[i]);
        if(len > 0 && production[i][len-1] == '\n')
            production[i][len-1] = '\0';
            
        // Check for valid production format (A=...)
        if(strlen(production[i]) < 2 || production[i][1] != '=') {
            printf("Invalid production format. Must be in form A=B. Try again.\n");
            i--;
        }
    }
    
    // Initialize arrays
    for(k = 0; k < MAX_PROD; k++) {
        for(i = 0; i < 100; i++) {
            calc_first[k][i] = '!';
            calc_follow[k][i] = '!';
        }
    }
    
    int point1 = 0, point2, xxx;
    char done[MAX_PROD];
    int ptr = -1;
    
    // Find first for all non-terminals
    for(k = 0; k < count; k++)
    {
        c = production[k][0];
        point2 = 0;
        xxx = 0;
        
        for(i = 0; i <= ptr; i++)
            if(c == done[i])
                xxx = 1;
        
        if (xxx == 1)
            continue;
        
        // Reset first array for each calculation
        n = 0;
        findfirst(c, 0, 0);
        
        ptr += 1;
        done[ptr] = c;
        printf("\n First(%c)= { ", c);
        calc_first[point1][0] = c;
        point2 = 1;
        
        for(i = 0; i < n; i++) {
            int lark = 0, chk = 0;
            for(lark = 1; lark < point2; lark++) {
                if (first[i] == calc_first[point1][lark]) {
                    chk = 1;
                    break;
                }
            }
            if(chk == 0) {
                printf("%c, ", first[i]);
                calc_first[point1][point2++] = first[i];
            }
        }
        printf("}\n");
        jm = n;
        point1++;
    }
    
    printf("\n");
    printf("-----------------------------------------------\n\n");
    
    // Find follow for all non-terminals
    char donee[MAX_PROD];
    ptr = -1;
    point1 = 0;
    int land = 0;
    
    for(k = 0; k < count; k++)
    {
        c = production[k][0];
        point2 = 0;
        xxx = 0;
        
        for(i = 0; i <= ptr; i++)
            if(c == donee[i])
                xxx = 1;
        
        if (xxx == 1)
            continue;
        
        land += 1;
        
        // Reset follow array for each calculation
        m = 0;
        follow(c);
        
        ptr += 1;
        donee[ptr] = c;
        printf(" Follow(%c) = { ", c);
        calc_follow[point1][0] = c;
        point2 = 1;
        
        for(i = 0; i < m; i++) {
            int lark = 0, chk = 0;
            for(lark = 1; lark < point2; lark++) {
                if (f[i] == calc_follow[point1][lark]) {
                    chk = 1;
                    break;
                }
            }
            if(chk == 0) {
                printf("%c, ", f[i]);
                calc_follow[point1][point2++] = f[i];
            }
        }
        printf(" }\n\n");
        km = m;
        point1++; 
    }
    
    // Find all terminals
    char ter[MAX_PROD];
    for(k = 0; k < MAX_PROD; k++) {
        ter[k] = '!';
    }
    
    int ap, vp, sid = 0;
    
    for(k = 0; k < count; k++) {
        for(i = 0; i < strlen(production[k]); i++) {
            if(!isupper(production[k][i]) && production[k][i] != '#' && production[k][i] != '=' && production[k][i] != '\0') {
                vp = 0;
                for(ap = 0; ap < sid; ap++) {
                    if(production[k][i] == ter[ap]) {
                        vp = 1;
                        break;
                    }
                }
                if(vp == 0 && sid < MAX_PROD-1) {
                    ter[sid] = production[k][i];
                    sid++;
                }
            }
        }
    }
    
    ter[sid] = '$';
    sid++;
    
    // Display parsing table header
    printf("\n\t\t\t The LL(1) Parsing Table for the above grammar:\n");
    printf("\n\t\t\t===================================================\n");
    printf("\t\t\t|\t");
    
    for(ap = 0; ap < sid; ap++) {
        printf("%c\t", ter[ap]);
    }
    
    printf("\n\t\t\t===================================================\n");
    
    // Calculate first of each production
    char first_prod[MAX_PROD][MAX_PROD];
    
    for(ap = 0; ap < count; ap++) {
        int destiny = 0;
        k = 2; // Skip A=
        int ct = 0;
        char tem[100];
        
        while(production[ap][k] != '\0') {
            if(!isupper(production[ap][k])) {
                tem[ct++] = production[ap][k];
                tem[ct++] = '_';
                tem[ct++] = '\0';
                k++;
                break;
            }
            else {
                int zap = 0;
                int tuna = 0;
                for(zap = 0; zap < count; zap++) {
                    if(calc_first[zap][0] == production[ap][k]) {
                        for(tuna = 1; tuna < 100; tuna++) {
                            if(calc_first[zap][tuna] != '!') {
                                tem[ct++] = calc_first[zap][tuna];
                            }
                            else
                                break;
                        }
                        break;
                    }
                }
                tem[ct++] = '_';
                k++;
            }
        }
        
        int zap = 0, tuna;        
        for(tuna = 0; tuna < ct; tuna++) {
            if(tem[tuna] == '#') {
                zap = 1;
            }
            else if(tem[tuna] == '_') {
                if(zap == 1) {
                    zap = 0;
                }
                else
                    break;
            }
            else {
                first_prod[ap][destiny++] = tem[tuna];
            }
        }
        first_prod[ap][destiny] = '\0';
    }
    
    // Create parsing table
    char table[MAX_PROD][MAX_PROD];
    ptr = -1;
    
    for(ap = 0; ap < MAX_PROD; ap++) {
        for(i = 0; i < MAX_PROD; i++) {
            table[ap][i] = '!';
        }
    }
    
    for(ap = 0; ap < count; ap++) {
        ck = production[ap][0];
        xxx = 0;
        
        for(i = 0; i <= ptr; i++)
            if(ck == table[i][0])
                xxx = 1;
        
        if (xxx == 1)
            continue;
        else {
            ptr = ptr + 1;
            table[ptr][0] = ck;
        }
    }
    
    // Fill parsing table
    for(ap = 0; ap < count; ap++) {
        int tuna = 0;
        while(first_prod[ap][tuna] != '\0') {
            int to, ni = 0;
            for(to = 0; to < sid; to++) {
                if(first_prod[ap][tuna] == ter[to]) {
                    ni = 1;
                    break;
                }
            }
            if(ni == 1) {
                char xz = production[ap][0];
                int cz = 0;
                while(cz <= ptr && table[cz][0] != xz) {
                    cz = cz + 1;
                }
                
                if(cz <= ptr) { // Make sure we found the non-terminal
                    int vz = 0;
                    while(vz < sid && ter[vz] != first_prod[ap][tuna]) {
                        vz = vz + 1;
                    }
                    
                    if(vz < sid) { // Make sure we found the terminal
                        table[cz][vz+1] = (char)(ap + 65);
                    }
                }
            }
            tuna++;
        }
    }
    
    // Handle epsilon productions
    for(ap = 0; ap < count; ap++) {
        int destiny = 0;
        int ct = 0;
        
        // Find if the production has epsilon
        int has_epsilon = 0;
        for(k = 2; production[ap][k] != '\0'; k++) {
            if(production[ap][k] == '#') {
                has_epsilon = 1;
                break;
            }
        }
        
        // Add entries for follow set if it has epsilon
        if(has_epsilon) {
            char xz = production[ap][0];
            int cz = 0;
            while(table[cz][0] != xz && cz <= ptr) {
                cz = cz + 1;
            }
            
            if(cz <= ptr) {
                // Find follow set for this non-terminal
                int fl;
                for(fl = 0; fl < point1; fl++) {
                    if(calc_follow[fl][0] == xz)
                        break;
                }
                
                if(fl < point1) {
                    for(i = 1; calc_follow[fl][i] != '!'; i++) {
                        int vz = 0;
                        while(vz < sid && ter[vz] != calc_follow[fl][i]) {
                            vz = vz + 1;
                        }
                        
                        if(vz < sid) {
                            table[cz][vz+1] = '#';
                        }
                    }
                }
            }
        }
    }
    
    // Display parsing table
    for(ap = 0; ap <= ptr; ap++) {
        printf("\t\t\t%c\t|", table[ap][0]);
        for(i = 1; i <= sid; i++) {
            if(table[ap][i] == '!')
                printf("\t");
            else if(table[ap][i] == '#')
                printf("%c=#\t", table[ap][0]);
            else {
                int mum = (int)(table[ap][i]);
                mum -= 65;
                if(mum >= 0 && mum < count)
                    printf("%s\t", production[mum]);
                else
                    printf("?\t");
            }
        }
        printf("\n");
        printf("\t\t\t---------------------------------------------------");
        printf("\n");
    }
    
    // Parser
    int j;
    printf("\n\nPlease enter the desired INPUT STRING (end with $): ");
    char input[100];
    fgets(input, 100, stdin);
    
    // Remove trailing newline
    int len = strlen(input);
    if(len > 0 && input[len-1] == '\n')
        input[len-1] = '\0';
    
    // Add $ if not present
    len = strlen(input);
    if(input[len-1] != '$') {
        input[len] = '$';
        input[len+1] = '\0';
    }
    
    printf("\n\t\t\t===========================================================================\n");
    printf("\t\t\t\tStack\t\t\tInput\t\t\tAction");
    printf("\n\t\t\t===========================================================================\n");
    
    int i_ptr = 0, s_ptr = 1;
    char stack[100];
    
    // Initialize stack
    stack[0] = '$';
    
    // Add start symbol
    stack[1] = table[0][0]; // First non-terminal
    stack[2] = '\0';
    
    while(s_ptr >= 0) {
        // Display stack
        printf("\t\t\t\t");
        for(i = 0; i <= s_ptr; i++) {
            printf("%c", stack[i]);
        }
        printf("\t\t\t");
        
        // Display remaining input
        for(i = i_ptr; input[i] != '\0'; i++) {
            printf("%c", input[i]);
        }
        printf("\t\t\t");
        
        // Get current symbols from stack and input
        char current_input = input[i_ptr];
        char top_stack = stack[s_ptr];
        
        // If stack empty except for $, check if input is also at $
        if(s_ptr == 0) {
            if(current_input == '$') {
                printf("Accept\n");
                printf("\n\t\t\tYOUR STRING HAS BEEN ACCEPTED!\n");
                break;
            } else {
                printf("Error: Stack empty but input not finished\n");
                printf("\n\t\t\tYOUR STRING HAS BEEN REJECTED!\n");
                break;
            }
        }
        
        // Pop the stack
        s_ptr--;
        
        // If terminal, match with input
        if(!isupper(top_stack) && top_stack != '#') {
            if(top_stack == current_input) {
                i_ptr++;
                printf("Match and pop %c\n", top_stack);
            } else {
                printf("Error: Terminal mismatch - expected %c, got %c\n", top_stack, current_input);
                printf("\n\t\t\tYOUR STRING HAS BEEN REJECTED!\n");
                break;
            }
        }
        // If non-terminal, lookup production
        else if(isupper(top_stack)) {
            // Find row in table
            int row = 0;
            while(row <= ptr && table[row][0] != top_stack) {
                row++;
            }
            
            if(row > ptr) {
                printf("Error: No entry in table for %c\n", top_stack);
                printf("\n\t\t\tYOUR STRING HAS BEEN REJECTED!\n");
                break;
            }
            
            // Find column for current input
            int col = 0;
            while(col < sid && ter[col] != current_input) {
                col++;
            }
            
            if(col >= sid) {
                printf("Error: Invalid input symbol %c\n", current_input);
                printf("\n\t\t\tYOUR STRING HAS BEEN REJECTED!\n");
                break;
            }
            
            // Get table entry
            char entry = table[row][col+1];
            
            if(entry == '!') {
                printf("Error: No production for %c with input %c\n", top_stack, current_input);
                printf("\n\t\t\tYOUR STRING HAS BEEN REJECTED!\n");
                break;
            } 
            else if(entry == '#') {
                printf("%c -> ε (epsilon)\n", top_stack);
                // Do nothing - epsilon production
            } 
            else {
                int prod_num = entry - 65;
                if(prod_num >= 0 && prod_num < count) {
                    printf("%s\n", production[prod_num]);
                    
                    // Push RHS in reverse order
                    int rhs_len = strlen(production[prod_num]) - 2; // Skip A=
                    char rhs[MAX_LEN];
                    strncpy(rhs, production[prod_num] + 2, rhs_len);
                    rhs[rhs_len] = '\0';
                    
                    // Epsilon production - don't push anything
                    if(strcmp(rhs, "#") == 0) {
                        continue;
                    }
                    
                    // Push in reverse order
                    for(i = strlen(rhs) - 1; i >= 0; i--) {
                        s_ptr++;
                        stack[s_ptr] = rhs[i];
                    }
                    stack[s_ptr + 1] = '\0';
                } else {
                    printf("Error: Invalid production index\n");
                    printf("\n\t\t\tYOUR STRING HAS BEEN REJECTED!\n");
                    break;
                }
            }
        }
        // If epsilon, do nothing
        else if(top_stack == '#') {
            printf("Apply epsilon production\n");
        }
    }
    
    printf("\n\t\t\t===========================================================================\n");
    
    return 0;
}

void follow(char c)
{
    int i, j;
    // If start symbol, add $ to follow
    if(production[0][0] == c) {
        f[m++] = '$';
    }
    
    // For all productions
    for(i = 0; i < count; i++)
    {
        // Check RHS of production
        for(j = 2; j < strlen(production[i]); j++)
        {
            if(production[i][j] == c)
            {
                // If not at end of production
                if(production[i][j+1] != '\0') {
                    followfirst(production[i][j+1], i, (j+2));
                }
                // If at end of production and not recursive
                if(production[i][j+1] == '\0' && c != production[i][0]) {
                    follow(production[i][0]);
                }
            }   
        }
    }
}

void findfirst(char c, int q1, int q2)
{
    int j;
    
    // If terminal, add to first
    if(!(isupper(c))) {
        first[n++] = c;
        return;
    }
    
    // Check all productions
    for(j = 0; j < count; j++)
    {
        if(production[j][0] == c)
        {
            // If epsilon production
            if(production[j][2] == '#') {
                if(production[q1][q2] == '\0')
                    first[n++] = '#';
                else if(production[q1][q2] != '\0' && (q1 != 0 || q2 != 0))
                {
                    findfirst(production[q1][q2], q1, (q2+1));
                }
                else
                    first[n++] = '#';
            }
            // If RHS starts with terminal
            else if(!isupper(production[j][2])) {
                first[n++] = production[j][2];
            }
            // If RHS starts with non-terminal
            else {
                findfirst(production[j][2], j, 3);
            }
        }
    }    
}

void followfirst(char c, int c1, int c2)
{
    int k;
    
    // If terminal, add to follow
    if(!(isupper(c)))
        f[m++] = c;
    else {
        int i = 0, j = 1;
        // Find first of this non-terminal
        for(i = 0; i < count; i++)
        {
            if(calc_first[i][0] == c)
                break;
        }
        
        // Add all elements from first except epsilon
        while(calc_first[i][j] != '!' && j < 100)
        {
            if(calc_first[i][j] != '#') {
                f[m++] = calc_first[i][j];
            }
            else {
                // If epsilon in first, look at what follows
                if(production[c1][c2] == '\0') {
                    follow(production[c1][0]);
                }
                else {
                    followfirst(production[c1][c2], c1, c2+1);
                }
            }
            j++;
        }
    }
}