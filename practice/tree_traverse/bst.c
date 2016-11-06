#include <stdio.h>
#include <stdlib.h>

#include "bst.h"

void visit_bst(BiTree node)
{
	printf("%d ", node->data);
}

void bst_toy(BiTree root) 
{

	BiTNode *left_1		= malloc(sizeof(BiTNode));
	BiTNode *right_1	= malloc(sizeof(BiTNode));
	BiTNode *right_2 	= malloc(sizeof(BiTNode));
	BiTNode *right_3 	= malloc(sizeof(BiTNode));
	BiTNode *left_2		= malloc(sizeof(BiTNode));

	*left_1 	= (BiTNode) { 2, NULL, right_2 };
	*right_1 	= (BiTNode) { 3, NULL, right_3};
	*right_2 	= (BiTNode) { 4, left_2, NULL };
	*left_2 	= (BiTNode) { 6, NULL, NULL };
	*right_3 	= (BiTNode) { 5,  NULL, NULL };

	root->data = 1;
	root->lchild = left_1;
	root->rchild = right_1;
}

void bst_toy_complete(BiTree root) 
{
	BiTNode *left_1 	= malloc(sizeof(BiTNode));
	BiTNode *right_1 	= malloc(sizeof(BiTNode));
	BiTNode *left_2 	= malloc(sizeof(BiTNode));
	BiTNode *right_2 	= malloc(sizeof(BiTNode));

	*left_1		= (BiTNode) { 2, left_2, right_2 };
	*right_1	= (BiTNode) { 3, NULL, NULL };
	*left_2 	= (BiTNode) { 4, NULL, NULL };
	*right_2	= (BiTNode) { 5, NULL, NULL };

	root->data = 1;
	root->lchild = left_1;
	root->rchild = right_1;
}