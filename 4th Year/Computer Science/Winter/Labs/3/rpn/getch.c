#include <stdio.h>
#include "getch.h"

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