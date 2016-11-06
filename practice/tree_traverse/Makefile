#
# Makefile for traverse algorithm 
#

CC = gcc
CFLAGS = -Wall -O2 -m32 -g

OBJS = traverse.o stack.o bst.o tbt.o

traverse: $(OBJS)
	$(CC) $(CFLAGS) -o traverse $(OBJS)

traverse.o: traverse.c tbt.h bst.h stack.h
tbt.o: tbt.c tbt.h
bst.o: bst.c bst.h
stack.o: stack.c stack.h bst.h

clean:
	rm -f *~ *.o traverse
