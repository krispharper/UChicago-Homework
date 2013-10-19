//Kris Harper
//kharper

#include <stdio.h>

int main()
{
	float fahren, cel;
	printf("Please enter a temperature in Fahrenheit: ");
	scanf("%f", &fahren);
	cel = 5*(fahren - 32)/9;
	printf("%2f F = %2f C\n", fahren, cel);
}
