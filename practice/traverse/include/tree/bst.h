#pragma once	/* make sure this header is included once, 
					both stack.h and queue.h need this header */

#include <stdio.h>

#define TRUE 1
#define FALSE 0

/* element type of data in node */
typedef int bst_elem_t;

/* 
 * struct definition of Binary Search Tree node
 */
typedef struct BiTNode {
	bst_elem_t data;	/* data */
	struct BiTNode *lchild; 	/* left child pointer */
	struct BiTNode *rchild;		/* right child pointer */
} BiTNode, *BiTree;

void visit_bst(BiTree node);
int search_bst(BiTree root, bst_elem_t a, BiTNode **x);
int insert_bst(BiTree *root, bst_elem_t a);	/* don't allow repeated key value */
void btree_toy(BiTree root);
void btree_toy_complete(BiTree root);
void bst_toy(BiTree root);
