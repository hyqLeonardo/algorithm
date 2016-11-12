#
# Makefile for tree-traverse algorithm 
# '-g' option in 'CFLAGS' is for debug
# '-I' set include path
#

CC = gcc
INC_DIR = ../../include/tree/
CFLAGS = -Wall -g -I$(INC_DIR)

OBJS = traverse.o stack.o queue.o bst.o tbt.o

traverse: $(OBJS)
	$(CC) $(CFLAGS) -o traverse $(OBJS)

# traverse.o: traverse.c tbt.h bst.h stack.h queue.h
# tbt.o: tbt.c tbt.h
# bst.o: bst.c bst.h
# stack.o: stack.c stack.h bst.h
# queue.o: queue.c queue.h bst.h

clean:
	rm -f *~ *.o traverse
