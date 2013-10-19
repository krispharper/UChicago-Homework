# Kris Harper
# kharper
# CMSC 15200
# Lab 3
# 1.18.11

# Top-level rule
all: rpn rpn_neg

# Compile getch
getch.o: getch.c getch.h

# Compile getop
getop.o: getop.c getop.h getch.h

# Compile getop_neg
getop_neg.o: getop_neg.c getop.h getch.h

# Compile stack
stack.o: stack.c stack.h

# Compile main
main.o: main.c getop.h stack.h

# Compile rpn
rpn: main.o stack.o getop.o getch.o
	gcc  main.o stack.o getop.o getch.o -o rpn

# Compile rpn_neg
rpn_neg: main.o stack.o getop_neg.o getch.o
	gcc  main.o stack.o getop_neg.o getch.o -o rpn_neg

# Phony link to clean
.PHONY: clean
clean:
	rm -f getch.o stack.o getop.o getop_neg.o main.o rpn rpn_neg