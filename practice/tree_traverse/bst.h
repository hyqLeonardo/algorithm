#include <stdio.h>

/* 
 * struct definition of Binary Search Tree node
 */
typedef struct BiTNode {
	int data;	/* data */
	struct BiTNode *lchild; 	/* left child pointer */
	struct BiTNode *rchild;		/* right child pointer */
} BiTNode, *BiTree;

void visit_bst(BiTree node);
void bst_toy(BiTree root);
void bst_toy_complete(BiTree root);
