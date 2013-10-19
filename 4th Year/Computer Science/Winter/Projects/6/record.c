/* implementation for a toy personnel record */

#include <assert.h>
#include "record.h"


RecP make_record(const char *fn, const char *ln, unsigned int loc)
{
  RecP p;
  if( (p = (RecP)malloc( sizeof(struct record)))){
    p->first_name = (char *) malloc( strlen(fn)+1 );
    p->last_name = (char *) malloc( strlen(ln)+1 );
    strcpy(p->first_name, fn);
    strcpy(p->last_name, ln);
    p->location = loc;
  }
  return p;
}

void free_record( RecP p )
{
  assert(p);
  free(p->first_name);
  free(p->last_name);
  free(p);
}

int less_first_name( const RecP a, const RecP b)
{return strcmp(a->first_name, b->first_name)<0;}

int less_last_name(  const RecP a, const RecP b)
{return strcmp(a->last_name, b->last_name)<0;}

int less_location(   const RecP a, const RecP b)
{return a->location < b->location;}

/* last name, then first name , then location */
int less_phonebook(   const RecP a, const RecP b)
{
	int fn = strcmp(a->first_name, b->first_name);
	int ln = strcmp(a->last_name, b->last_name);
	int loc = a->location - b->location;

	if(ln > 0)
		return 0;
	if(ln == 0){
		if(fn > 0)
			return 0;
		if(fn == 0){
			if(loc > 0)
				return 0;
			return 1;
		}
		return 1;
	}
	return 1;
}
