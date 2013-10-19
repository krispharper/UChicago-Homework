// Lab 4: dynamic array
// Paolo Codenotti
// paoloc

#include <stdio.h>
#include <stdlib.h>


struct vector{
	int *pa;       // pointer to the array
	int last;      // last element assigned a value
	int nElts;     // number of elements in the array
	int capacity;  // capacity of the array
};

void print_array (struct vector *pv);

int main()
{
	/* Initialize the struct to an empty array
	 * Set last to -1 so that when we print we print nothing, 
	 * and there is no seg_fault for accessing a NULL pointer.
	 */
	struct vector v = {NULL, -1, 0, 0}; 
	int *new_pa;
	int pos, val, i, new_capacity;

	do{
		printf("Enter a position\n");
		printf("(-1 to end the program, -2 to print the array, -3 to print the number of elements):");
		scanf("%d", &pos);

		if(pos == -1)
			break; 
		else if (pos == -2){
			print_array (&v);
			continue;
		}     
		else if (pos == -3){
			printf("The number of element currently assigned is: %d\n", v.nElts);
			continue;
		}
		else {
			printf("Enter a value: ");
			scanf("%d", &val);

			if (pos >= v.capacity) 			/* Must resize the array */
			{	   
				/* Allocate a new array with sufficient size */
				new_capacity = 2 * (pos+1); 
				new_pa = (int *) calloc (new_capacity, sizeof(int));
				
				if ( new_pa == NULL ){
					printf("Error allocating memory!\n");
					return 1;
				}

				/* Initialize new array: copy old array, and initialize rest */
				for (i = 0; i < v.capacity; i++)
					new_pa[i] = v.pa[i];

				for (i = v.capacity; i < new_capacity; i++)
					new_pa[i] = -1;

				/* Free memory and update variables */
				free(v.pa);
				v.pa = new_pa;
				v.capacity = new_capacity;
			}

			/* Update nElts and last if necessary */
			if(v.pa[pos]!=-1)
				v.nElts++;	

			/* This if will be true if pos >= v.capacity (old capacity)
			 * since we did not update last */
			if (pos >= v.last) 
				v.last = pos;
	
			/* Do the assignment */
			v.pa[pos] = val;
		}
	} while (pos != -1);
  
	print_array(&v);
	return 0;
}

void print_array (struct vector *pv){
	int i;
	
	for(i = 0; i <= pv->last; i++){
		if(pv->pa[i]==-1)
			printf("array[%d] = [No value assigned]\n", i);
		else
			printf("array[%d] = %d\n", i, pv->pa[i]);
	}
}
