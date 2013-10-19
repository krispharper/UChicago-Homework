//Kris Harper
//kharper

#include <stdio.h>

int main()
{
	float fahren, cel;
	int exit;
	while (exit != 2)
	{
		exit = 0;
		printf("Please enter a temperature in Fahrenheit: ");
		scanf("%f", &fahren);
		cel = 5*(fahren - 32)/9;
		printf("%2f F = %2f C\n", fahren, cel);
		printf("\nWould you like to enter another temperature (Please type 1 for Y, 2 for N)? ");
		scanf("%d", &exit);
	}
}
