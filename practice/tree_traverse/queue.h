// #pragma once
/* 
 * queue implementation
 */

#include <stdio.h>
#include <stdlib.h>

#include "bst.h"

#define MAXSIZE 50
#define TRUE 1
#define FALSE 0

/*
 * struct definition of array based queue
 */
typedef struct a_queue {
	BiTree data[MAXSIZE];
	int front;	/* position of head element */
	int rear;	/* position of tail element */
} a_queue;


void init_queue(a_queue *queue);
int queue_empty(a_queue *queue);
int enqueue(a_queue *queue, BiTree a);
int dequeue(a_queue *queue, BiTree *x);

/* strcut definition of link based queue */
typedef struct l_node {
	BiTree data;
	struct l_node *next;
} l_node;

typedef struct l_queue {
	struct l_node *front;
	struct l_node *rear;
} l_queue;

void init_lqueue(l_queue *queue);
int lqueue_empty(l_queue *queue);
void enlqueue(l_queue *queue, BiTree a);	/* can't fail */
int delqueue(l_queue *queue, BiTree *x);
