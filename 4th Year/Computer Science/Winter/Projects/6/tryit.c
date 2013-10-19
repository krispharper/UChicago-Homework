/* test heapsort of a vector of records */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "heap_sort.h"
#include "record.h"


int get_records( RecP a[], int maxRecord ); /* read from stdin */
void makeRandonNumerArray(int len, RecP a[]);
void print_records( char *rmk, int len, RecP a[] );
void fprint_records( FILE *outp, char *rmk, int len, RecP a[] );

int main()
{

  const int maxRecord = 100000;
  int ncmp;
  int len;
  RecP personnel[maxRecord];
  FILE *numout;

  time_t t;

  len = get_records( personnel, maxRecord);
  print_records( "\nThe list of personnel is as follows;\n",
                 len, personnel);

  ncmp =  heap_sort( len, personnel, less_first_name);
  print_records("\nAfter first-name sort the list is as follows:\n ",
             len, personnel);
  ncmp += heap_sort( len, personnel, less_last_name);
  print_records("\nAfter last-name sort the list is as follows:\n ",
             len, personnel);
  ncmp += heap_sort( len, personnel, less_location);

  printf("\nSorting this list took %d comparisons, total.\n ",
         ncmp );
  print_records("\nAfter the location sort the list is as follows:\n ",
                len, personnel);

  ncmp = heap_sort( len, personnel, less_phonebook);
  printf("\nSorting this list for phonebook took %d comparisons.\n ",
         ncmp );
  print_records("\nAfter the phonebook sort the list is as follows:\n ",
                len, personnel);

  while (--len >= 0 )
    free_record(personnel[len]);

  time(&t);
  srand(clock()+t%10);
  len = 1000;
  RecP *numbers = personnel;
  makeRandonNumerArray(len, numbers);
  ncmp = heap_sort(len, numbers, less_location);
  printf("\nSorting 1000 numbers took %d comparisons.\n",
         ncmp);
  numout = fopen("numout.txt", "w");
  fprint_records(numout,"After sorting the list is \n", len, numbers); 

  len = 100000;
  makeRandonNumerArray(len, numbers);
  ncmp = heap_sort(len, numbers, less_location);
  printf("\nSorting 100,000 numbers took %d comparisons.\n",
         ncmp);
  ncmp = heap_sort(len, numbers, less_location);
  printf("\nRe-sorting 100,000 numbers took %d comparisons.\n",
         ncmp);

  return 0;
}

/* read a vector of records from stdin */
/* we assume that the first and last names */
/* have no more than 255 characters */
/* return the number of records read */
int get_records(RecP v[], int max_vsize )
{
  char first[256];
  char last[256];
  int loc;
  int len;

  for( len=0;
       scanf("%s %s %d", first, last, &loc) != EOF;
       len++ ){
    v[len] = make_record(first,last,loc);
    if( len == max_vsize-1 ){
      len++;
      break;
    }
  }
  return len;
}

/* make a vector of records with random integer part */
void makeRandonNumerArray( int len, RecP v[])
{
  int i;
  for(i=0; i<len; i++ )
    v[i] = make_record("","",rand()%100000);
}

void print_records( char *rmk, int len, RecP v[] )
{
  fprint_records(stdout, rmk, len, v);
}

void fprint_records( FILE * outp, char *rmk, int len, RecP v[] )
{
  int i;
  fprintf(outp, "\n%s\n", rmk);
  for(i=0; i<len; i++)
    fprintf( outp, "%-15s %-15s %10d\n", v[i]->first_name, 
             v[i]->last_name, v[i]->location);
}
