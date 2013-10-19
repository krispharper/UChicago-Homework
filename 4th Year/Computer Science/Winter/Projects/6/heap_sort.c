#include "record.h"
#include "heap_sort.h"

int heap_sort(int len, RecP a[], r_cmp less)
{
	// find the last parent node
	int start = len/2 - 1;
	int cmp = 0;
	// continually call sift_down on each parent node
	while(start >= 0){
		sift_down(len-1, a, less, start, &cmp);
		start -= 1;
	}

	while(len-1  > 0){
		swap(a, len-1, 0);
		sift_down(len-2, a, less, 0, &cmp);
		len -= 1;
	}

	return cmp;
}
void sift_down(int len, RecP a[], r_cmp less, int i, int *cmp)
{
	int child, swap_node;

	// continue while the root has a child
	while((child = i*2 + 1) <= len){
		swap_node = i;
		// compare root and left child
		if(less(a[swap_node], a[child])){
			swap_node = child;
			*cmp += 1;
		}
		// compare swap and right child, if it exits
		if(child < len && less(a[swap_node], a[child+1])){
			swap_node = child + 1;
			*cmp += 1;
		}
		// now check to see if we need to make a swap
		if(swap_node != i){
			swap(a, i, swap_node);
			i = swap_node;
		}
		else
			return;
	}
}

void swap(RecP a[], int i, int j)
{
	RecP temp;
	temp = a[i];
	a[i] = a[j];
	a[j] = temp;
}
