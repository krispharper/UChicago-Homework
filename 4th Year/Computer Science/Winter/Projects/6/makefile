CCFLAGS = -g -Wall
CC = gcc

OBJS = \
   heap_sort.o\
   record.o\
   tryit.o

heap_sorttest : $(OBJS) 
	$(CC) $(CCFLAGS) -o $@ $(OBJS)

heap_sort.o : heap_sort.c heap_sort.h record.h
	$(CC) -c $(CCFLAGS) heap_sort.c

record.o : record.c record.h
	$(CC) -c $(CCFLAGS) record.c

tryit.o : tryit.c heap_sort.h
	$(CC) -c $(CCFLAGS) tryit.c


spotless : 
	rm heap_sorttest *~ *.o numout.txt
