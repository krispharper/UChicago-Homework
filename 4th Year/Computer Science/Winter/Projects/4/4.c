// Kris Harper
// CMSC 15200
// Project 3
// 1.24.11

#include <stdio.h>
#include <math.h>
#include <string.h>

struct point {
	double x;
	double y;
};

// Make a typedef for a pointer to a function which takes a double and returns a struct point
typedef struct point (*funct)(double);

// Set global variables for the function evals and depth
int evals = 0;
static int level = 0;
char *table_line = "*-----------*-----------*----------*-------------*\n";

main()
{
	struct point line(double t), parabola(double t), sqrt_sine(double t);
	void print_row(double a, double b, funct f, char *name);

	// Print a header for the table and then the table rows using print_row
	printf("%s", table_line);
	printf("|Curve      |Tolerance  |Length    |Evaluations  |\n");
	printf("%s", table_line);
	print_row(0.0, 2.0, line, "Line");
	print_row(0.0, 1.0, parabola, "Parabola");
	print_row(0.0, 1.0, sqrt_sine, "Sqrt_sine");
}

// Prints each row of the table
void print_row(double a, double b, funct f, char *name)
{
	double curve_length(double a, double b, funct f, double tol);
	double lowtol = .01, hightol = .0001;
	int i, len = (int) strlen(name);

	// Print the function name and then spaces to fill in the entry
	printf("|");
	for (i=0; i<len; i++){
			printf("%c", *name);
			name++;
	}
	for (; i<11; i++)
		printf(" ");
	// Make a call to curve_length
	printf("|%0.2lf       |%lf  |", lowtol, curve_length(a, b, f, lowtol));
	printf("%d", evals);
	for (i=0; i<13-digits(evals); i++)
		printf(" ");
	// Reset evals and depth counters to 0 for the next call to curve_length
	evals = 0;
	level = 0;
	printf("|\n|           ");
	printf("|%.4lf     |%lf  |", hightol, curve_length(a, b, f, hightol));
	printf("%d", evals);
	for (i=0; i<13-digits(evals); i++)
		printf(" ");
	evals = 0;
	level = 0;
	printf("|\n%s", table_line);
}

// Find the distance between two points
double dist(struct point pt1, struct point pt2)
{
	double sqrt(double x), root;
	root = sqrt((pt1.x - pt2.x)*(pt1.x - pt2.x) + (pt1.y - pt2.y)*(pt1.y - pt2.y));
	return root;
}

// Find the number of digits in a positive integer
int digits(int n)
{
	int num;
	while(n > 0){
		num++;
		n/=10;
	}
	return num;
}

double curve_length(double a, double b, funct f, double tol)
{
	double coarse, fine, mid;
	const int minlevel = 3, maxlevel = 30;
	struct point origin = {0, 0};
	struct point fa = f(a), fb = f(b), fmid;
	
	// Increase the level for each call, increase the evals by 3 for the 2 calls above and one below
	level++;
	evals += 3;
	mid = (a+b)/2;
	fmid = f(mid);
	coarse = dist(fa, fb);
	fine = dist(fa, fmid) + dist(fmid, fb);
	
	// If it's fine enough and we've done enough iterations, return fine, else keep going
	if (((fine - coarse) <= tol || level == maxlevel) && level > minlevel){
		level--;
		return fine;
	}
	else 
		return curve_length(a, mid, f, tol/2) + curve_length(mid, b, f, tol/2);
}

// Three test curves
struct point line(double t)
{
	struct point output = {t, 2*t};
	return output;
}

struct point parabola(double t)
{
	struct point output = {t, t*t};
	return output;
}

struct point sqrt_sine(double t)
{
	struct point output = {t, sqrt(sin(t))};
	return output;
}