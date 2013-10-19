// Paolo Codenotti

/* file stack.c
 * implementation of push and pop for a stack of doubles.
 */

#include<stdio.h>

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
