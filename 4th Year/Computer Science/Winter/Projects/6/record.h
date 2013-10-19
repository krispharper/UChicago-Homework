/* struct for a toy personnel record */
#ifndef P_RECORD_H
#define P_RECORD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct record{
  char *first_name;
  char *last_name;
  unsigned int location;
};

typedef struct record *RecP;
typedef int (*r_cmp)(const RecP, const RecP );

RecP make_record( const char *fn,
                  const char *ln,
                  unsigned int loc);

void free_record( RecP p );

/* comparisons for records */
int less_first_name( const RecP a, const RecP b);
int less_last_name(  const RecP a, const RecP b);
int less_location(   const RecP a, const RecP b);
int less_phonebook(   const RecP a, const RecP b);

#endif
