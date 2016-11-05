#
# Makefile for sorting algorithm 
#

CC = gcc
CFLAGS = -Wall -O2 -m32 -g

OBJS = link_list.o

traverse: $(OBJS)
	$(CC) $(CFLAGS) -o link_list $(OBJS)

traverse.o: link_list.c

clean:
	rm -f *~ *.o link_list

