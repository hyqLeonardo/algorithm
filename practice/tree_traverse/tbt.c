#include <stdio.h>
#include <stdlib.h>

#include "tbt.h"

void visit_thread(ThreadTree node)
{
	printf("%d ", node->data);
}

/* use in-order traverse to construct threads */
void in_thread(ThreadTree p, ThreadTree *prev)
// careful, prev must be a pointer to struct pointer, cause it will be 
// modified inside function call !
{
	if (p) {
		// printf("enter node %d\n", p->data);
		in_thread(p->lchild, prev);
		if (p->lchild == NULL) {
			p->lchild = *prev;	/* lchild is NULL, set thread to prev */
			p->ltag = 1;
			// printf("node %d ltag modified, lchild is now %d\n", 
				// p->data, p->lchild->data);
		}
		// printf("prev node %d\n", (*prev)->data);
		if ((*prev)->data != 0 && (*prev)->rchild == NULL) {	/* prev->data == 0 means prev is NULL */
			(*prev)->rchild = p;
			(*prev)->rtag = 1;
			// printf("node %d rtag modified, rchild is now %d\n", 
				// (*prev)->data, (*prev)->rchild->data);
		}
		*prev = p;
		in_thread(p->rchild, prev);
		// printf("leave node %d\n", p->data);
	}
}

void build_in_thread(ThreadTree T) 
{
	ThreadTree prev = malloc(sizeof(ThreadNode));
	if (T) {
		in_thread(T, &prev);
		prev->rchild = NULL;	/* deal with last node */
		prev->rtag = 1;
	}
}

void thread_toy(ThreadTree root)
{
	ThreadNode *left_1		= malloc(sizeof(ThreadNode));
	ThreadNode *right_1		= malloc(sizeof(ThreadNode));
	ThreadNode *right_2 	= malloc(sizeof(ThreadNode));
	ThreadNode *right_3 	= malloc(sizeof(ThreadNode));
	ThreadNode *left_2		= malloc(sizeof(ThreadNode));

	*left_1 	= (ThreadNode) { 2, NULL, right_2 };
	*right_1 	= (ThreadNode) { 3, NULL, right_3};
	*right_2 	= (ThreadNode) { 4, left_2, NULL };
	*left_2 	= (ThreadNode) { 6, NULL, NULL };
	*right_3 	= (ThreadNode) { 5,  NULL, NULL };

	root->data = 1;
	root->lchild = left_1;
	root->rchild = right_1;

	build_in_thread(root);	/* create in-order threads */
}

ThreadNode *first_node(ThreadNode *p)
{
	while (p->ltag == 0)
		p = p->lchild;
	return p;
}

ThreadNode *next_node(ThreadNode *p)
{
	if (p->rtag == 1)
		return p->rchild;
	else {
		return first_node(p->rchild);
	}
}