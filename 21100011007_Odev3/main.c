// 211000110007 Fatma Gizem ARI odev 3

/*Bu kodda dosyadan infix ifade alip onu adim adim postfixe çeviriyorum.
islem sirasinda farkli seyler var gibi olsa da cevrim dogru yapiliyor
sonrasında postfix hale gelmis ifadenin sonucu adim adim hesaplanıyor
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#define MAX 100

typedef struct {
    char data[MAX];
    int top;
} Stack;

typedef struct {
    int data[MAX];
    int top;
} IntStack;

void initStack(Stack *s)
{
    s->top=-1;
}

void initIntStack(IntStack *s)
{
    s->top= -1;
}

int isEmpty(Stack *s)
{
    return s->top== -1;
}

int isIntStackEmpty(IntStack *s)
{
    return s->top== -1;
}

int isFull(Stack *s)
{
    return s->top==MAX-1;
}

int isIntStackFull(IntStack *s)
{
    return s->top==MAX-1;
}

void push(Stack *s,char val)
{
    if (!isFull(s))
    {
        s->data[++(s->top)]=val;
    }
}

void pushInt(IntStack *s,int val)
{
    if (!isIntStackFull(s))
    {
        s->data[++(s->top)]=val;
    }
}

char pop(Stack *s)
{
    if (!isEmpty(s))
    {
        return s->data[(s->top)--];
    }
    return -1;
}

int popInt(IntStack *s)
{
    if (!isIntStackEmpty(s))
    {
        return s->data[(s->top)--];
    }
    return -1;
}

char peek(Stack *s)
{
    if (!isEmpty(s))
    {
        return s->data[s->top];
    }
    return -1;
}

int precedence(char op)
{
    if (op=='*' || op=='/')
    {
        return 2;
    }
    if (op=='+' || op=='-')
    {
        return 1;
    }
    return 0;
}

int isHigherPrecedence(char op1, char op2)
{
    return precedence(op1) >= precedence(op2);
}

void printStep(char* postfix, Stack* s,int step)
{
    printf("\nAdim %d:\n",step);
    printf("Postfix: %s\n",postfix);
    printf("Stack: ");
    for (int i=0; i<=s->top;i++)
    {
        printf("%c ", s->data[i]);
    }
    printf("\n");
}

int isOperator(char c,char operators[])
{
    for (int i=0;i<4;i++)
    {
        if (c==operators[i])
        {
            return 1;
        }
    }
    return 0;
}

void infixToPostfix(char* infix,char* postfix)
{
    Stack s;
    initStack(&s);
    char operators[4]={'+','-','*','/'};
    int i=0,j=0;
    int step=1;
    char prevPostfix[MAX]="";

    while(infix[i]!='\0' && infix[i]!='\n')
    {
        char siradaki=infix[i];
        if(isdigit(siradaki))
        {
            while(isdigit(infix[i]))
            {
                postfix[j++]=infix[i++];
            }
            postfix[j++]=' ';
            i--;
        }
        else if(siradaki=='(')
        {
            push(&s,siradaki);
        }
        else if(siradaki==')')
            while (!isEmpty(&s) && peek(&s)!='(')
            {
                postfix[j++]=pop(&s);
                postfix[j++]=' ';
                pop(&s);
        }
        else if(isOperator(siradaki,operators))
        {
            while(!isEmpty(&s) && isHigherPrecedence(peek(&s),siradaki))
            {
                postfix[j++]=pop(&s);
                postfix[j++]= ' ';
            }
            push(&s,siradaki);
        }
        i++;
        printStep(postfix,&s,step++);
    }

    while(!isEmpty(&s))
        postfix[j++]=pop(&s);
        postfix[j++]=' ';

        if (strcmp(prevPostfix,postfix)!=0)
        {
            printStep(postfix,&s,step++);
            strcpy(prevPostfix,postfix);
        }
    postfix[j]='\0';
}

int evaluatePostfix(char* postfix)
{
    IntStack s;
    initIntStack(&s);
    int i=0;
    int step=1;

    printf("\nPostfix islemi adimlari:\n");

    while(postfix[i]!='\0')
        {
        if(isdigit(postfix[i]))
        {
            int num=0;
            while(isdigit(postfix[i]))
            {
                num=num*10+(postfix[i]-'0');
                i++;
            }
            pushInt(&s,num);
            printf("Adim %d: %d yigina eklendi\n",step++,num);
        }
        else if(postfix[i]==' ')
        {
            i++;
        }
        else if(isOperator(postfix[i],"+-*/"))
        {
            int val2=popInt(&s);
            int val1=popInt(&s);
            int result;
            switch(postfix[i])
            {
                case '+': result=val1 + val2; break;
                case '-': result=val1 - val2; break;
                case '*': result=val1 * val2; break;
                case '/': result=val1 / val2; break;
            }
            printf("Adim %d: %d %c %d = %d, sonuc yigina eklendi\n",step++,val1,postfix[i],val2,result);
            pushInt(&s,result);
            i++;
        }
    }

    return popInt(&s);
}

int main()
{
    char infix[MAX],postfix[MAX];
    FILE *file;

    file=fopen("infix.txt","r");
    if (file==NULL)
    {
        printf("Dosya acilamadi!\n");
        return 1;
    }

    fgets(infix,MAX,file);
    fclose(file);

    printf("Okunan Infix ifade: %s\n",infix);

    infixToPostfix(infix, postfix);

    printf("\nSon Postfix ifade: %s\n",postfix);

    int result = evaluatePostfix(postfix);
    printf("\nPostfix hesaplama sonucu: %d\n",result);

    return 0;
}
