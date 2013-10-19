#include <stdio.h>

#define size 10

void make_heap(int *list);
void sift_down(int *list, int start, int end);
void swap(int *list, int i, int j);

int main()
{
	int list[size], end = size-1, i;

	printf("Please enter 10 integers each followed by a return\n");
	for(i=0; i < size; i++)
		scanf("%d", &list[i]);

	make_heap(list);

	while(end > 0){
		swap(list, end, 0);
		sift_down(list, 0, end-1);
		end -= 1;
	}

	printf("\n");
	printf("The integers in order are:\n");
	for(i=0; i < size; i++)
		printf("%d ", list[i]);
	printf("\n");

	return 0;
}

void make_heap(int *list)
{
	// find the last parent node
	int start = size/2 - 1;
	// continually call sift_down on each parent node
	while(start >= 0){
		sift_down(list, start, size-1);
		start -= 1;
	}
}

void sift_down(int *list, int root, int end)
{
	int child, swap_node;

	// continue while the root has a child
	while((child = root*2 + 1) <= end){
		swap_node = root;
		// compare root and left child
		if(list[swap_node] < list[child])
			swap_node = child;
		// compare swap and right child, if it exits
		if(child < end && list[swap_node] < list[child+1])
			swap_node = child + 1;
		// now check to see if we need to make a swap
		if(swap_node != root){
			swap(list, root, swap_node);
			root = swap_node;
		}
		else
			return;
	}
}
// swap: interchange v[i] and v[j]
void swap(int *list, int i, int j)
{
	int temp;
	temp = list[i];
	list[i] = list[j];
	list[j] = temp;
}
