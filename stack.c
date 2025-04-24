#include<stdio.h>
#include<stdbool.h>
#include<ctype.h>
#include<stdlib.h>
#define MAX 100

int stk[MAX],top=-1;


void Push(){
    int val;
    if(top == MAX-1){
        printf("StackOverFlow\n");
    }
    printf("Enter the element\n");
    scanf("%d",&val);
    stk[++top]=val;
}

void Pop(){
    if(top == -1){
        printf("StackUnderFlow\n");
        return;
    }
    printf("Popped ELement %d\n",stk[top--]);
}

void Display(){
    if(top==-1){
        printf("The Stack is Empty...\n");
        return;
    }
    for(int i=top;i>=0;i--){
        printf("%d\t",stk[i]);
    }
    printf("\n");
}

void Peek(){
    if(top==-1){
        printf("The stack is empty..\n");
        return;
    }
    printf("The top Element is %d\n",stk[top]);
}

int main(){
    // stk[0] = -1;
    printf("This is a program to execute stack...");
    start:
    printf("\n1.Push\n2.Pop\n3.Display\n4.peek\n5. Exit\n");
    int n;
    scanf("%d",&n);
    switch(n){
        case 1:
            Push();
            goto start;
            break;
        case 2:
            Pop();
            goto start;
            break;
        case 3:
            Display();
            goto start;
            break;
        case 4:
            Peek();
            goto start;
            break;
        case 5:
            exit(0);
        deafult:
            printf("Enter a valid number...\n");goto start;
    }
}
