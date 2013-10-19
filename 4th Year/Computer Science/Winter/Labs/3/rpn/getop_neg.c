// Paolo Codenotti
// Reverse Polish Notation Calculator
// From Khernigham and Ritchie 

/* file getop.c: 
 * implements the function getop, which  returns the next operation or operand
 */

#include <stdio.h> // for EOF
#include <ctype.h>

#include"getop.h"
#include"getch.h"

/* getop: get next oper */
int getop (char s[])
{
	int i, c;
	char next;
	
	while((s[0] = c = getch()) == ' ' || c == '\t')
		;
	s[1] = '\0';
	if (!isdigit(c) && c != '.' && c != '-')
		return c;	// not a number
	if(c == '-'){
		c = getch();
		ungetch(c);
		if(!isdigit(c) && c != '.')
			return '-';
	}
		
	i=0;
	if (isdigit(c))		// collect integer part
		while (isdigit(s[++i] = c = getch()))
			;
	if (c == '.')		// collect fractional part
		while (isdigit(s[++i] = c = getch()))
			;
	s[i] = '\0';
	if(c != EOF )
		ungetch(c);
	return NUMBER;
}

