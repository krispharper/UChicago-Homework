// Kris Harper
// CMSC 15200
// Project 3
// 1.24.11

#include <stdio.h>

main(int argc, char *argv[])
{
	void print_month(int month, int year);
	int month = atoi(argv[1]);
	int year = atoi(argv[2]);
	
	if ((month < 1 || month > 12) || year < 0){
		printf("Please enter a valid month and year as arguments\n");
		return 0;
	}
	else
		print_month(month, year);
}

void print_month(int month, int year)
{
	int ymo = year - 1, i, days=0, dow = 0, start = 0;
	int month_length(int n, int year);
	char *month_name(int n);
	
	// Find the number of days in the year before the first day of the month
	for (i = 1; i < month; i++){
		days += month_length(i, year);
	}
	// Find the day of week which the first of the month lands on
	dow = (1 + days + 365*ymo + ymo/4 - ymo/100 + ymo/400) % 7;
	
	// Print the header information
	printf("    %s, %d\n", month_name(month), year);
	printf("  S  M  T  W  R  F  S\n");
	
	// Print spaces until the right day of week
	for (i = 0; i < dow; i++)
		printf("   ");
	
	// Print the lines for each week, putting a new line every seven entries
	for (i = 0; i < month_length(month, year); i++, dow++){
		if ((dow % 7) == 0 && start != 0)
			printf("\n");
		start++;
		(i > 8) ? printf(" %d", i + 1) : printf("  %d", i + 1);
	}
	printf("\n");

}

// Return the number of days in a given month
int month_length(int n, int year)
{
	static int length[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	if (n < 1 || n > 12)
		return 0;
	else if (n == 2 && isleapyear(year))
		return length[n-1] + 1;
	else
		return length[n-1];
}

// Return name of n-th month
char *month_name(int n)
{
	static char *name[] = {
	"Illegal month",
	"January",
	"February",
	"March",
	"April",
	"May",
	"June",
	"July",
	"August",
	"September",
	"October",
	"November",
	"December"
	};
	return (n < 1 || n > 12) ? name[0] : name[n];
}

// Determines if a year is a leap year or not

int isleapyear(int year)
{
	if ((year % 4 == 0 && year %100 != 0) || year % 400 == 0)
		return 1;
	else
		return 0;
}