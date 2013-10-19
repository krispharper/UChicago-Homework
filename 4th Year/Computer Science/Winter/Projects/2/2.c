// Kris Harper
// CMSC 15200
// Project 2
// 1.19.11

#include <stdio.h>
#include <math.h>

#define MAXIMUM 100
#define MAX_DATA_SIZE 1000
#define numInt 10

main()
{
	double data[MAX_DATA_SIZE];
	double c, total = 0, var = 0;
	int size=0, outs = 0, i, bins, binsize = MAXIMUM/numInt;
	void qsort(double v[], int left, int right);
	double median(double data[], int size);
	
	// read in the data
	while (scanf("%lf", &c) != EOF){
		// take care of outliers and count them as outs
		if (c > MAXIMUM){
			c = MAXIMUM;
			outs++;
		}
		else if (c < 0){
			c = 0;
			outs++;
		}
		data[size++] = c;
		// form a total
		total += c;
	}
	// sort the data
	qsort(data, 0, size-1);
	
	// find the mean and standard deviation
	// var is the variance (almost)
	double mean = total/size;
	for (i = 0; i < size; ++i)
		var += (data[i] - mean) * (data[i] - mean);
		
	double stdd = sqrt(var/size);
	
	
	// form the output
	printf("There were %d data items.\n", size);
	if (outs > 0)
		printf("Found %d point(s) out of bounds.\n", outs);
	printf("Mean: %.2lf\n", mean);
	printf("Standard deviation: %.2lf\n", stdd);
	printf("Median: %.2lf\n", median(data, size));
	// make the histogram using the fact that data is already sorted
	int j = size - 1;
	for (bins = 0; bins < numInt; ++bins){
		printf("%d - %d ", MAXIMUM - (bins+1)*binsize, MAXIMUM - bins*binsize);
		while (data[j] >= MAXIMUM - (bins+1)*binsize && j >= 0){
			printf("X");
			j--;
		}
		printf("\n");
	}
}

// qsort: sort v[left]...v[right] into increasing order
void qsort(double v[], int left, int right)
{
	int i, last;
	void swap(double v[], int i, int j);

	if (left >= right) 					// do nothing if array contains
		return; 						// fewer than two elements
	swap(v, left, (left + right)/2); 		// move partition elem
	last = left;							// to v[0]
	for (i = left + 1; i <= right; i++) // partition
		if (v[i] < v[left])
			swap(v, ++last, i);
	swap(v, left, last);				//restore partition elem
	qsort(v, left, last-1);
	qsort(v, last+1, right);
}

// swap: interchange v[i] and v[j]
void swap(double v[], int i, int j)
{
	int temp;
	temp = v[i];
	v[i] = v[j];
	v[j] = temp;
}

// compute the median
double median(double data[], int size)
{
	if ((size % 2) == 0)
		return (data[size/2] + data[size/2 - 1])/2;
	else
		return data[size/2];
}
