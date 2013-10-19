#include <stdio.h>

int main()
{
	float miles, km;
	printf("Type number of mules: ");
	scanf("%f", &miles);
	km = miles * 1.61;
	printf("Kilometers: %f \n", km);
}
