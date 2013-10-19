//Kris Harper
//CMSC 15200
//Project 1
//1.12.10

#include <stdio.h>


//Determines if a year is a leap year or not
isleapyear(int year)
{
	if ((year % 4 == 0 && year %100 != 0) || year % 400 == 0)
		return 1;
	else
		return 0;
}

//Find the doomsday for the given year
doomsday(int year)
{
	//Get the first two digits of year
	int c = (year - (year % 100))/100 + 1;
	
	//Find the anchor day for the century of the year
	int anchor = (5*c + (c-1)/4 + 4) % 7;
	
	int y = year % 100;
	
	//Calculates the Doomsday based on Conway's algorithm
	int doomsday = (y/12 + y % 12 + (y % 12)/4 + anchor) % 7;
	
	return doomsday;
}

//This program uses Conway's Doomsday algorithm to find isoyears.
//The Doomsday is a day of the week (represented as 0-6) which anchors the year.
//Two years have the same calendar if they have the same Doomsday and
//the same value for isleapyear.
int main()
{
	int year, i, j;
	printf("Enter a year: ");
	scanf("%d", &year);
	
	printf("\nYear | Isoyears\n");
	printf("----------------\n");
	//Use two nested for loops to make a table. The first indexes the years
	//and the second indexes the isoyears
	for (i = year; i > year - 10; i--) {
		printf("%d |", i);
		for (j = i-10; j <= i+10; j++) {
			//Print an isoyear if i and j have the same doomsday and isleapyear
			if ((isleapyear(i) == isleapyear(j)) && (doomsday(i) == doomsday(j)) && (i != j))
				printf(" %d ", j);
		}
		printf("\n");
	}
}