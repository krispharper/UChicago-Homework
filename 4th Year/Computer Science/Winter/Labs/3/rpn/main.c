// Paolo Codenotti
// Reverse Polish Notation Calculator
// From Khernigham and Ritchie 

#include<stdio.h>
#include<stdlib.h>	// for atof()

#include"getop.h"
#include"stack.h"

#define MAXOP 100	// max size of operand or operator 


/* reverse Polish calculator */
main()
{
	int type;
	double op2;
	char s[MAXOP];
	
	printf("\nWelcome to the reverse Polish notation calculator rpn.\n");
	printf("Please enter a reverse Polish notation expression to compute (Q or q to quit)\n\n> ");
	
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
				printf("\t %g\n> ", pop());
				break;
			case 'q':
			case 'Q':
				return 0;
				break;
			default:
				printf("error: unknown command\n");
				break;
		}				
	}
	return 0;
}
