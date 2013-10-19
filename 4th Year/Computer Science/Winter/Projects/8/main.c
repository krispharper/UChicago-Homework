// Kris Harper
// CMSC 15200
// Project 8
// 2.23.11

/* process priority queue and hash table */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
/* struct for a toy process data */

typedef struct process process;
typedef process *proc_ptr;

struct process{
  unsigned int number; /* process identifier */
  int priority; 
  int owner;
  proc_ptr next; /* hash table link */
  unsigned int index; /* index in array rep of priority queue */
};

proc_ptr make_process( unsigned int num, int prior, int own,
                       proc_ptr nxt );
proc_ptr add_process( unsigned int num, int prior, int own);
unsigned int hash( unsigned int n );
proc_ptr *find( unsigned int num );
void sift_up_pq( int i );
void sift_down_pq( int i);
void swap_pq( int i, int j );
void remove_process( proc_ptr p );
void change_priority( proc_ptr p, int change );
void check_pq();
void print_h_table();

#define NHASH 173
#define MULTIPLIER 31
#define MAXPROCESSES 1000 /* max number processes in queue */

/* arrays for process hash table and priority queue */
proc_ptr p_table[NHASH];
proc_ptr p_queue[MAXPROCESSES];

/* current number of processes */
unsigned int num_procs = 0;

/* the purpose of this main is to show that we
   can add processes, remove processes, and
   change the priority of processes, while
   maintaining a priority queue based on
   process priority and a hash table based
   on process number.
*/
int main()
{
  unsigned int i, k,  pri, hidx,pnum;
  int numprocess = 100;
  proc_ptr p;

  for( i=0; i< numprocess; i++){
    pri =  ((double)rand()/RAND_MAX)*16.0 + 0.5;
    add_process( i,              /* process number */
                 pri,            /* priority */
                 0);             /* owner */
    check_pq();
    if( i%5 == 0 ){ /* remove top process */
      remove_process( p_queue[0] );
      check_pq();
    }
    if( num_procs == 0 )
      continue;
    if( i%7 == 0 ){ /* change a priority */
      k = num_procs /3;
      p = p_queue[k];
      pnum = p->number;
      if( p !=  *find(pnum))
        printf( "Failure in find.\n");
      if( i%2 ==  0 )
        change_priority(p,5);
      else
        change_priority(p,-5);
      check_pq();
    }
    if( i%19 == 0 ){ /* remove a non top process */
      k = num_procs/2 + 3;
      if( k >= num_procs )
        k = num_procs -1;
      remove_process(p_queue[k]);
      check_pq();
    }
  }
  print_h_table();

  while( num_procs > 0){
    remove_process(p_queue[0]);
    check_pq();
  }

  return 0;
}

/* make a new process and put it in  prioity queue */
proc_ptr make_process( unsigned int num, int prior, int own,
                       proc_ptr nxt )
{
  if( num_procs >= MAXPROCESSES ){
    printf("Too many processes; new one not created\n");
    return nxt;
  }
  proc_ptr temp = malloc(sizeof(process));
  temp->number = num;
  temp->priority = prior;
  temp->owner = own;
  temp->next = nxt;
  temp->index = num_procs;
  p_queue[num_procs] = temp;
  num_procs++;
  sift_up_pq(num_procs-1);
  return temp;
}

/* use make_procees to crate a process. put it in the */
/* hash table */
proc_ptr add_process( unsigned int num, int prior, int own)
{
  int index = hash(num);
  proc_ptr temp = make_process(num, prior, own, NULL);

  if(p_table[index] == NULL){
  	p_table[index] = temp;
	return temp;
  }
  
  proc_ptr current = p_table[index];
  while(current->next != NULL){
  	current = current->next;
  }
  current->next = temp;
  return temp;
}

/* map process number into [0,NHASH-1] */
unsigned int hash( unsigned int n )
{
  unsigned int i, v=n>>16;
  for( i=0; i<4; i++ ){
    v = MULTIPLIER * v + n % 16;
    n /= 16;
  }
  return v % NHASH;
}

/* given a process number, find the address of */
/* the pointer in the hash table to that process */
/* return NULL if the process is not found */
proc_ptr *find( unsigned int num )
{
  proc_ptr *temp;

  for(temp = &p_table[hash(num)]; *temp != NULL; temp = &(*temp)->next)
    if((*temp)->number == num)
      return temp;
  return NULL;
}

/* do sift_up in p_queue */
void sift_up_pq( int i )
{
  int j;
  while(i > 0){
    j = (i-1)/2;
    if(p_queue[j]->priority < p_queue[i]->priority){
      swap_pq(i, j);
      i = j;
    }
    else
      return;
  }
}

/* do sift_down in p_queue */
void sift_down_pq( int i)
{
  int swap_node, j;
  while((j = i*2+1) <= num_procs-1){
    swap_node = i;
    if(p_queue[swap_node]->priority < p_queue[j]->priority)
      swap_node = j;
    if(j < num_procs && p_queue[swap_node]->priority < p_queue[j+1]->priority)
      swap_node = j + 1;
    if(swap_node != i){
      swap_pq(i, swap_node);
      i = swap_node;
    }
    else
      return;
  }
}

/* swap the processes at locations i & j in p_queue */
void swap_pq( int i, int j )
{
  proc_ptr temp;
  int i_index = p_queue[i]->index, j_index = p_queue[j]->index;
  temp = p_queue[i];
  p_queue[i] = p_queue[j];
  p_queue[j] = temp;
  p_queue[i]->index = i_index;
  p_queue[j]->index = j_index;
}

/* remove a process from hash table and priority queue */
/* do not call with p = NULL */
void remove_process( proc_ptr p )
{
  proc_ptr temp;
  temp = p_table[hash(p->number)];
  if(temp->next == NULL)
    p_table[hash(p->number)] = NULL;
  else {
    while(temp->next != NULL){
     if(temp->next->number == p->number)
      temp->next = temp->next->next;
     else
      temp = temp->next;
    }
  }

  int i, j=0;

  for(i=p->index; i<num_procs; i++){
    p_queue[i] = p_queue[i+1];
    if(i != num_procs-1)
      (p_queue[i]->index)--;
  }
  num_procs--;
  while(j < num_procs){
  	sift_up_pq(j);
		j++;
	}
}

/* change the priority of *p and maintian priority queue */
void change_priority( proc_ptr p, int change )
{
  p->priority = change;
 	sift_up_pq(p->index);
}

/* check that root has a maximal priority */
/* report failure, but not success */
void check_pq()
{
  int i,max_p;
  proc_ptr p;
  if( num_procs == 0 )
    return;
  max_p = p_queue[0]->priority;
  for( i=0; i<num_procs; i++ ){
    p = p_queue[i];
    if( p->index != i )
      printf(" index_messed up\n");
    if( p->priority > max_p )
      max_p = p->priority;
    if( find(p->number) == NULL )
      printf("find failure\n");
  }
  if( max_p > p_queue[0]->priority )
    printf("root fails to have the max priority\n");
}

/* print the process numbers in non-empty hash buckets */
void print_h_table()
{
  int i;
  proc_ptr temp;

  for(i=0; i<NHASH; i++){
  	printf("[%d] ", i);
    temp = p_table[i];
    while(temp != NULL){
      printf("%d ", temp->number);
      temp = temp->next;
    }
    printf("\n");
  }
}
