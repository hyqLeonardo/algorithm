#include <stdio.h>
#include <stdlib.h>

#include "bst.h"

void visit_bst(BiTree node)
{
	printf("%d ", node->data);
}

int search_bst(BiTree root, int a, BiTNode **x)
{
	while (root) {
		if (root->data == a) {
			*x = root;
			return TRUE;
		}
		if (root->data >= a)
			return search_bst(root->lchild, a, x);
		else
			return search_bst(root->rchild, a, x);
	}
	return FALSE;
}

int insert_bst(BiTree *root, int a)
{
	if (*root == NULL || (*root)->data == 0) {
		BiTNode *node = malloc(sizeof(BiTNode));
		node->data = a;
		*root = node;	/* insert here */
		return TRUE;
	}
	else if ((*root)->data == a)
		return FALSE;
	else if ((*root)->data > a)
		return insert_bst(&((*root)->lchild), a);
	else
		return insert_bst(&((*root)->rchild), a); 
}

/* btree_toy :
 *
 *         1
 *        / \
 *       /   \
 *      /     \
 *     2       3
 *      \       \
 *       4       5
 *      /       
 *     6       
 */
void btree_toy(BiTree root) 
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

/* btree_toy complete :
 * 
 *         1
 *        / \
 *       /   \
 *      /     \
 *     2       3
 *    / \      
 *   4   5      
 */
void btree_toy_complete(BiTree root) 
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

/* bst_toy :
 *
 *         5
 *        / \
 *       /   \
 *      /     \
 *     2       8
 *    / \     / \
 *   1   4   7   10
 *      /   /   / 
 *     3   6   9
 */
void bst_toy(BiTree root) 
{
	root->data = 5;
	insert_bst(&root, 2);
	insert_bst(&root, 8);
	insert_bst(&root, 1);
	insert_bst(&root, 4);
	insert_bst(&root, 7);
	insert_bst(&root, 10);
	insert_bst(&root, 3);
	insert_bst(&root, 6);
	insert_bst(&root, 9);
}
