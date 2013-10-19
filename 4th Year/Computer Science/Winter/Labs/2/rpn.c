// Paolo Codenotti
// Reverse Polish Notation Calculator
// From Khernigham and Ritchie 

#include<stdio.h>
#include<stdlib.h>	// for atof()

#define MAXOP 100	// max size of operand or operator 
#define NUMBER '0'	// signal that a number was found

int getop(char[]);
void push(double);
double pop(void);

/* reverse Polish calculator */
main()
{
	int type;
	double op2;
	char s[MAXOP];
	
	while ( (type = getop(s)) != EOF) {
		switch (type) {
		case NUMBER:
			push(atof(s));
			break;
		case '+':
			push(pop() + pop());
			break;
		case '*':
			push(pop() * pop());
			break;
		case '-':
			op2 = pop();
			push(pop() - op2);
			break;
		case '/':
			op2 = pop();
			if (op2 != 0.0)
				push(pop() / op2);
			else
				printf("error: division by zero\n");
			break;
		case '\n':
				printf("  %.8g\n", pop());
			break;
		case 'q':
			return 0;
		case 'Q':
			return 0;
		default:
			printf("error: unknown command\n");
			break;
		}
	}
	return 0;
}

#define MAXVAL 100		// maximum depth of value stack

int sp = 0;				// next free stack position
double val[MAXVAL];		// value stack

/* push: push f onto the value stack */
void push(double f)
{
	if(sp < MAXVAL)
		val[sp++] = f;
	else
		printf("error: stack full, can't push %g\n", f);
}

/* pop: pop and return top values from stack */
double pop(void)
{
	if(sp>0)
		return val[--sp];
	else {
		printf("error: popping from an empty stack\n");
		return 0.0;
	}
}

#include <ctype.h>

int getch(void);
void ungetch(int);

/* getop: get next oper */
int getop (char s[])
{
	int i, c;
	
	while((s[0] = c = getch()) == ' ' || c == '\t')
		;
	
	s[1] = '\0';
	if (!isdigit(c) && c != '.')
		return c;	// not a number
	i=0;
	if (isdigit(c))		// collect integer part
		while (isdigit(s[++i] = c = getch()))
			;
	if (c == '.')		// collect fractional part
		while (isdigit(s[++i] = c = getch()))
			;
	s[i] = '\0';
	if(c != EOF)
		ungetch(c);
	return NUMBER;
}

#define BUFSIZE 100

char buf[BUFSIZE];	// buffer for ungetch
int bufp = 0;		// next free position in buffer

/* get a (possibly pushed back) character */
int getch(void)
{
	return(bufp > 0) ? buf[--bufp] : getchar();
}

/* push character back on input */
void ungetch(int c)
{
	if(bufp >= BUFSIZE)
		printf("error: can't ungetch, too many characters\n");
	else
		buf[bufp++] = c;
}