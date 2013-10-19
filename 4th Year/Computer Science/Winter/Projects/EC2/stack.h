/* interface for stacks and queues of ints */
#ifndef STACK_QUEUE_INT
#define STACK_QUEUE_INT

/* stacks and queues should always be constructed using */
/* the appropriate make_ function and freed using the   */
/* corresponding free_ function                         */


/* === === === === === === === === === === === === === === */
/* list member for stack and queue implementation */
struct _sll_int{
  int data;
  struct _sll_int *next;
};
typedef struct _sll_int sll_int;

/* make a new sll_int object */
/* return true iff successful */
sll_int *make_sll_int( int i, sll_int *n);

/* free an sll_int object */
void free_sll_int( sll_int *p);


/* === === === === === === === === === === === === === === */
/* last in first out, LIFO, container for ints */
struct _stack_int{
  sll_int *top;
};
typedef struct _stack_int stack_int;

/* make an empty stack */
/* returns NULL iff it fails */
stack_int *make_stack_int();

/* free a stack */
void free_stack_int( stack_int *sp );

/* push an item onto the stack */
/* return true iff successful */
/* stack is unchanged if memory is exhausted */
int stack_int_push( int i, stack_int *sp );

/* remove an entry from the stack */
/* undefined on empty stacks */
int stack_int_pop( stack_int *sp);

/* return true iff the stack is empty */
int stack_int_empty( const stack_int *sp );

/* return the number of entries in the stack */
int stack_int_size( const stack_int *sp);


/* === === === === === === === === === === === === === === */
/* first in first out, FIFO, container for ints */
struct _queue_int{
  sll_int *head;
  sll_int *tail; /* point to last element */
};
typedef struct _queue_int queue_int;

/* make an empty queue */
/* returns NULL if it fails */
queue_int *make_queue_int();

/* free a queue */
void free_queue_int( queue_int *qp );

/* push an item onto the queue */
/* return true iff successful */
int queue_int_push( int i, queue_int *qp );

/* remove an entry from the queue */
/* undefined on empty queues */
int queue_int_pop( queue_int *qp);

/* return true iff the queue is empty */
int queue_int_empty( const queue_int *qp );

/* return the number of entries in the queue */
int queue_int_size( const queue_int *qp);

#endif
