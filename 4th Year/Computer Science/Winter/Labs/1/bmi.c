//Kris Harper
//kharper

#include <stdio.h>

int main()
	{
	int age = 0, gender = 0, weight = 0;
	float height = 0;
	
	while (age <= 0)
		{
		printf("What is your age? ");
		scanf("%d", &age);
		if (age <= 0)
		 	{
		 	printf("Please enter a valid age.\n");
		 	}
		}
		
	while (!(gender == 1 || gender == 2))
		{
		printf("\nWhat is your gender? (F=1 or M=2) ");
		scanf("%d", &gender);
		if (!(gender == 1 || gender == 2))
			{
			printf("Please enter either 1 or 2.\n");
			}
		}
	
	while (weight <= 0)
		{
		printf("\nWhat is your weight? (kg) ");
		scanf("%d", &weight);
		if (weight <= 0)
			{
			printf("Please enter a valid weight.\n");
			}
		}
	
	while (height <=0 )
		{
		printf("\nWhat is your height? (m) ");
		scanf("%f", &height);
		if (height <= 0)
			{
			printf("Please enter a valid height.\n");
			}
		}
			
	if (age <= 18)
		{
		printf("You're too young to be registered at UofC!\n");
		}
	else
		{
		float bmi = weight/(height*height);
		if (gender == 1)
			{
			if (bmi <= 20.7)
				{
				printf("Underweight\n");
				}
			else if (20.7 < bmi && bmi < 26.4)
				{
				printf("Ideal weight\n");
				}
			else
				{
				printf("Overweight\n");
				}
			}
		else if (gender == 2)
			{
			if (bmi <= 19.1)
				{
				printf("Underweight\n");
				}
			else if (19.1 < bmi && bmi < 25.8)
				{
				printf("Ideal weight\n");
				}
			else
				{
				printf("Overweight\n");
				}
			}
		}
	}
