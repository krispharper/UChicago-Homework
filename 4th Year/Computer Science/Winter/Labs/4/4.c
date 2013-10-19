// Kris Harper
// kharper
// CMSC 15200
// Lab 4
// 1.25.11

#include <stdio.h>
#include <stdlib.h>

struct array{
    int *p;
    int num;
    int last;
    int size;
};

int main()
{
  /* YOU WILL HAVE TO CHANGE THESE DECLARATIONS */
	void print_array(struct array a);
	int *init = NULL, *new;
	int size=0;
	struct array a = {init, 0, 0, size};
	int pos, val, i=0;

	do
	{      
		printf("Enter a position\n");
		printf("(-1 to end the program, -2 to print the array, -3 to print the number of elements):");
		scanf("%d", &pos);

		if(pos == -1)
			break; 
		else if(pos == -2){
			print_array(a);
			continue;
      	}     
		else if(pos == -3){
			printf("Number of elements entered is: %d\n", a.num);
			continue;
		}
		else {
			printf("Enter a value: ");
			scanf("%d", &val);
			if (pos >= a.size){
				new = malloc((pos+1)*2*sizeof(int));
				while(i < a.size){
					new[i] = a.p[i];
					i++;
				}
				free(a.p);
				a.p = new;
				a.size = (pos+1)*2;
	    	}
		a.num++;
		a.last = pos;
		a.p[pos] = val;
		}

	}
	while (pos != -1);
  
print_array(a);
}

void print_array(struct array a)
{
	int i = 0;
//	printf("%d\n", a.size);
	
	for(i=0; i < a.size; i++){
		if(a.p[i] > 0)
			printf("array[%d] = %d\n", i, a.p[i]);
		else
			printf("array[%d] = [No value assigned]\n", i);
	}
}