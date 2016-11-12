#pragma once

#include <stdio.h>

/* 
 * struct definition of Threaded Binary Tree node
 */
typedef struct ThreadNode {
	int data;	/* data */
	struct ThreadNode *lchild; 		/* left child pointer */
	struct ThreadNode *rchild;		/* right child pointer */
	int ltag, rtag;
} ThreadNode, *ThreadTree;

void visit_thread(ThreadTree node);
void in_thread(ThreadTree p, ThreadTree *prev);
void build_in_thread(ThreadTree T);
ThreadNode *first_node(ThreadNode *p);
ThreadNode *next_node(ThreadNode *p);
void thread_toy(ThreadTree root);
