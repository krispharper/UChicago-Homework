// Kris Harper and Nora Sandler
// kharper and nlsandler
// CMSC 15200
// Lab 2
// 1.11.11


// When we run the program as given, it gives an error message: "unknown command".
// It doesn't appear to recognize any of the cases.
// It doesn't work because there's no case for \n.

// getch() will only stop reading in when it's read one character too far.
// ungetch() removes that extraneous character.
 

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
	printf("Please enter an expression in reverse Polish notation.\n");
	printf("The operator should immediately follow the operands.\n");
	printf("Press q to quit.\n");
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
		case '%':
			op2 = pop();
			push ((int) pop() % (int) op2);
			break;
		case '\n':
			printf("   %.8g\n", pop());
			printf("Please enter an expression in reverse Polish notation.\n");	
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
	int i, c, d;
	
	while((s[0] = c = getch()) == ' ' || c == '\t')
		;
	
	s[1] = '\0';
	i=0;
	if (!isdigit(c) && c != '.'){ 			// not a number
		if (c == '-'){						// adds an extra case for negative numbers
			if (isdigit(c = getch()))
				s[++i] = c;
			else{
				ungetch(c);				
				return '-';
			}		
		}
		else
			return c;
	}
	if (isdigit(c))		// collect integer part while checking for exponentials
		while (isdigit(s[++i] = c = getch()) || c == 'e' || (c == '-' && s[i-1] == 'e'))
			;
	if (c == '.')		// collect fractional part
		while (isdigit(s[++i] = c = getch()) || c == 'e')
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
