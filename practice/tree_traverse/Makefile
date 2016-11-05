#
# Makefile for traverse algorithm 
#

CC = gcc
CFLAGS = -Wall -O2 -m32 -g

OBJS = traverse.o stack.o bst.o

traverse: $(OBJS)
	$(CC) $(CFLAGS) -o traverse $(OBJS)

traverse.o: traverse.c bst.h stack.h
bst.o: bst.c bst.h
stack.o : stack.c stack.h bst.h

clean:
	rm -f *~ *.o traverse
