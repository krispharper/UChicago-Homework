/* use heap sort to sort an array of records */
#ifndef HEAP_SORT_RECORDS
#define HEAP_SORT_RECORDS
#include "record.h"

/* a complete binary tree (CBT) is represented */
/* as an array, a[], with the parent of a[i]   */
/* being a[(i-1)/2], the left child of a[i]    */
/* being a[2*i+1] and its right child being    */
/* a[2*i+2]                                    */

/* a CBT is a heap if the parent is at least   */
/* as large as its children                    */

int heap_sort( int len, RecP a[], r_cmp less );
void sift_down( int len, RecP a[], r_cmp less, int i, int *cmp );
void swap( RecP a[], int i, int j );

#endif
