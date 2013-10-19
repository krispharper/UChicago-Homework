// Kris Harper
// CMSC 15200
// Project 3
// 1.24.11

#include <stdio.h>
#include <string.h>

main(int argc, char *argv[])
{
	int month, year;
	void print_month(int month, int year);
	void print_year(int year);
	
	// Conditional tree with error handling and direction
	if(argc < 2 || argc > 3){
		printf("Usage: %s (month) year\n", argv[0]);
		return 0;
	}
	else {
		if(argc == 3){
			month = atoi(argv[1]);
			year = atoi(argv[2]);
			if((month < 1 || month > 12) || year < 0){
				printf("Please enter a valid month and year as arguments\n");
				return 0;
			}
			else {
				print_month(month, year);
				return 0;
			}
		}
		else if(argc == 2){
			year = atoi(argv[1]);
			if(year < 0){
				printf("Please enter a positive year\n");
				return;
			}
			else {
				print_year(year);
			}
		}
	}
}

// Given a day of month and month of year, print the appropriate week starting on that day
int print_week(int day, int month, int year)
{
	int dow=day, i;
	
	// Find the day of the year and day of week that day lands on
	for(i=1; i < month; i++)
		dow += month_length(i, year);
	dow = (dow + 365*(year-1) + (year-1)/4 - (year-1)/100 + (year-1)/400) % 7;
	
	// Print the week, increasing the day of the month as we go
	for(i=0; i < dow; i++)
		printf("   ");
	for(i=dow; i < 7 && day <= month_length(month); i++){
		if(i==6)
			(day < 10) ? printf(" %d", day++) : printf("%d", day++);
		else
			(day < 10) ? printf(" %d ", day++) : printf("%d ", day++);
	}
	// Print out extra spaces at the end of the month
	if(day > month_length(month)){
		for(; i < 7; i++)
			(i == 6) ? printf("  ") : printf("   ");
	}
	// Return the next day of the month
	return day;
}

void print_month(int month, int year)
{
	int day = 1, len = month_length(month, year);
	void print_header(int month, int year);
	int print_week(int day, int month, int year);
	
	// Print the header information
	print_header(month, year);
	printf("Su Mo Tu We Th Fr Sa\n");
	// Loop over the days in the month, printing out the weeks as we go
	while(day <= len){
		day = print_week(day, month, year);
		printf("\n");
	}
}

void print_year(int year)
{
	int day1, day2, day3, i, j;
	void print_header(int month, int year);
	int print_week(int day, int month, int year);
	int month_length(int month, int year);
	int digits(int n);
	
	// Print the header information
	for(i=1; i < (64-digits(year))/2; i++)
		printf(" ");
	printf("%d", year);
	for(i += digits(year); i < 65; i++)
		printf(" ");
	printf("\n\n");

	// Loop over the four rows
	// Each loop loops over the three months which are in a print_week while loop
	// The date in each of the three months is kept track of in day1, day2 and day3
	for(i=1; i <= 12; i += 3){
		day1=1;
		day2=1;
		day3=1;
		print_header(i+12, year);
		while(day1 <= month_length(i, year) &&
				day2 <= month_length(i+1, year) &&
				day3 <= month_length(i+2, year)){
			for(j=0; j < 3; j++){
				day1 = print_week(day1, i, year);
				printf("  ");
				day2 = print_week(day2, i+1, year);
				printf("  ");
				day3 = print_week(day3, i+2, year);
				printf("\n");
			}
		}
	}
}

// Print the header information for a month or a year
// A special month is used for the year case where 13-24 correspond to January to December
void print_header(int month, int year)
{
	int len, i, j;
	int digits(int n);
	char *month_name(int n);
	char *days = "Su Mo Tu We Th Fr Sa";
	
	// Break between month and year cases
	if(month < 13){
		len = strlen(month_name(month)) + 1 + digits(year);
		
		for(i=0; i < (21-len)/2; i++)
			printf(" ");
		printf("%s %d", month_name(month), year);
		for(i+=len; i < 21; i++)
			printf(" ");
	}
	// Print three month names in a row, padded with spaces
	else {
		for(j=0; j < 3; j++){
			len = strlen(month_name(month-12+j));
			for(i=0; i < (20-len)/2; i++)
				printf(" ");
			printf("%s", month_name(month-12+j));
			for(i+=len; i < 21; i++)
				printf(" ");
			if(j < 2)
				printf("  ");
		}
		printf("\n%s  %s  %s", days, days, days);
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

// Find the number of digits in a positive integer
int digits(int n)
{
	int num = 0;
	while(n > 0){
		num++;
		n/=10;
	}
	return num;
}