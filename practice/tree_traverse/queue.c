#include <stdio.h>

#include "queue.h"

/* array based queue */
void init_queue(a_queue *queue)
{
	queue->front = 0;
	queue->rear = 0;
}

int queue_empty(a_queue *queue)
{
	if (queue->front == queue->rear)
		return TRUE;
	else
		return FALSE;
}

int enqueue(a_queue *queue, BiTree a)
{
	if ((queue->rear+1)%MAXSIZE == queue->front)	/* queue is full */
		return FALSE;
	queue->data[queue->rear] = a;
	queue->rear = (queue->rear+1)%MAXSIZE;
	return TRUE;
}

int dequeue(a_queue *queue, BiTree *a)
{
	if (queue_empty(queue))
		return FALSE;
	*a = queue->data[queue->front];
	queue->front = (queue->front+1)%MAXSIZE;
	return TRUE;	
}

/* link based queue, with header node */
void init_lqueue(l_queue *queue)
{
	l_node *header = malloc(sizeof(l_node));
	header->next = NULL;
	queue->front = header;
	queue->rear = header;
}

int lqueue_empty(l_queue *queue)
{
	if (queue->front == queue->rear)
		return TRUE;
	else
		return FALSE;
}

void enlqueue(l_queue *queue, BiTree a)
{
	/* create new node */
	l_node *node = malloc(sizeof(l_node));
	node->data = a;
	node->next = NULL;

	queue->rear->next = node;
	queue->rear = node;
}

int delqueue(l_queue *queue, BiTree *x)
{
	if (lqueue_empty(queue))
		return FALSE;

	l_node *node = queue->front->next;
	(*x)->data = node->data->data;	/* careful */
	(*x)->lchild = node->data->lchild;	/* must assign this, cause x was just malloced */
	(*x)->rchild = node->data->rchild;

	queue->front->next = node->next;
	if (queue->rear == node) 	/* only has one node */
		queue->rear = queue->front;	/* now queue is empty */
	free(node);	/* delete node */
	return TRUE;
}