#
# Makefile for sorting algorithm 
#

CC = gcc
CFLAGS = -Wall -O2 -m32 -g

OBJS = sort.o

traverse: $(OBJS)
	$(CC) $(CFLAGS) -o sort $(OBJS)

traverse.o: sort.c

clean:
	rm -f *~ *.o sort
