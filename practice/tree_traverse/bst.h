#pragma once	/* make sure this header is included once, 
					both stack.h and queue.h need this header */

#include <stdio.h>

#define TRUE 1
#define FALSE 0

/* 
 * struct definition of Binary Search Tree node
 */
typedef struct BiTNode {
	int data;	/* data */
	struct BiTNode *lchild; 	/* left child pointer */
	struct BiTNode *rchild;		/* right child pointer */
} BiTNode, *BiTree;

void visit_bst(BiTree node);
int search_bst(BiTree root, int a, BiTNode **x);
int insert_bst(BiTree *root, int a);	/* don't allow repeated key value */
void btree_toy(BiTree root);
void btree_toy_complete(BiTree root);
void bst_toy(BiTree root);
