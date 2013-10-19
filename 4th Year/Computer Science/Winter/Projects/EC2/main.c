// short test for stack_int and queue_int
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
/*  unsigned int i;
  stack_int *sp = make_stack_int();
  queue_int *qp = make_queue_int();

  for(i=0; i<11; i++ ){ // put stuff in *sp and *qp
    stack_int_push( i, sp );
    queue_int_push( i, qp );
  }
  printf("The stack is of size %d\n", stack_int_size(sp));
  printf("The queue is of size %d\n", queue_int_size(qp));
  printf("\n");

  printf("The stack gives back:");
  while( !stack_int_empty(sp) )
    printf( " %d",stack_int_pop(sp));
  printf("\n");

  printf("The queue gives back:");
  while( !queue_int_empty(qp) )
    printf( " %d",queue_int_pop(qp));
  printf("\n\n");

  printf("The stack is of size %d\n", stack_int_size(sp));
  printf("The queue is of size %d\n", queue_int_size(qp));

  free_stack_int( sp );
  free_queue_int( qp );
*/
  return 0;
}

// stacks and queues should always be constructed using
// the appropriate make_ function and freed using the
// corresponding free_ function


// make a new sll_int object
// return true iff successful
sll_int *make_sll_int(int i, sll_int *n)
{
  sll_int temp = {i, n};
  sll_int *tempp;
  if((tempp = (sll_int) malloc(sizeof(sll_int))) != NULL){
    *tempp = temp;
    return tempp;
  }
  else
    return NULL;
}

// free an sll_int object
void free_sll_int(sll_int *p)
{
  sll_int *n = p->next;
  do {
    n = p->next;
    free(p);
  } while(n != 0);
}

/*
// make an empty stack
// returns NULL iff it fails
stack_int *make_stack_int();

// free a stack
void free_stack_int( stack_int *sp );

// push an item onto the stack
// return true iff successful
// stack is unchanged if memory is exhausted
int stack_int_push( int i, stack_int *sp );

// remove an entry from the stack
// undefined on empty stacks
int stack_int_pop( stack_int *sp);

// return true iff the stack is empty
int stack_int_empty( const stack_int *sp );

// return the number of entries in the stack
int stack_int_size( const stack_int *sp);


// make an empty queue
// returns NULL if it fails
queue_int *make_queue_int();

// free a queue
void free_queue_int( queue_int *qp );

// push an item onto the queue
// return true iff successful
int queue_int_push( int i, queue_int *qp );

// remove an entry from the queue
// undefined on empty queues
int queue_int_pop( queue_int *qp);

// return true iff the queue is empty
int queue_int_empty( const queue_int *qp );

// return the number of entries in the queue
int queue_int_size( const queue_int *qp);*/