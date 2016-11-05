/*
 * traverse algorithms
 */

#include <stdio.h>
#include <stdlib.h>

#define MaxSize 50
#define TRUE 1
#define FALSE 0

/* 
 * struct definition of tree node
 */
typedef struct BiTNode {
	int data;	/* data */
	struct BiTNode *lchild; 	/* left child pointer */
	struct BiTNode *rchild;		/* right child pointer */
} BiTNode, *BiTree;

void visit(BiTree node)
{
	printf("%d ", node->data);
}

/*
 * struct definition of array based stack
 */
typedef struct a_stack {
	BiTree data[MaxSize];
	int top;	/* position of top element */
} a_stack;

void init_stack(a_stack *stack)
{
	(*stack).top = -1;
}

int stack_empty(a_stack *stack)
{
	return stack->top == -1 ? TRUE : FALSE;
}

int push(a_stack *stack, BiTree a)
{
	if (stack->top == MaxSize-1)
		return FALSE;
	stack->data[++stack->top] = a;
	return TRUE; 
}

int pop(a_stack *stack, BiTree *x)
{
	if (stack->top == -1)
		return FALSE;
	*x = stack->data[stack->top--];
	return TRUE;		
}

int get_top(a_stack *stack, BiTree *x)
{
	if (stack->top == -1)
		return FALSE;
	*x = stack->data[stack->top];
	return TRUE;
}

/* traverse using recursion */
void pre_order(BiTree T)
{
	if (T != NULL) {
		visit(T);
		pre_order(T->lchild);
		pre_order(T->rchild);
	}
}

void in_order(BiTree T)
{
	if (T != NULL) {
		in_order(T->lchild);
		visit(T);
		in_order(T->rchild);
	}
}

void post_order(BiTree T)
{
	if (T != NULL) {
		post_order(T->lchild);
		post_order(T->rchild);
		visit(T);
	}
}

/* traverse using stack structure */
void pre_order_2(BiTree T)
{
	/* can initialize a stack first, then create a pointer to that */
	// a_stack s;
	// init_stack(&s);
	// a_stack *stack = &s;
	/* same as above, initialize, then create pointer */
	// a_stack s = { .top = -1 };
	// a_stack *stack = &s;
	/* Or, use malloc to allocate a memory size of struct 
	 * where malloc's return value is the pointer to stack struct */
	a_stack *stack = malloc(sizeof(a_stack));
	init_stack(stack);
	BiTree p = T;

	while (p || !stack_empty(stack)) {
		if (p) {
			push(stack, p);
			visit(p);
			p = p->lchild;
		}
		else {
			pop(stack, &p);
			p = p->rchild;
		}
	}
}

void in_order_2(BiTree T)
{
	a_stack *stack = malloc(sizeof(a_stack));
	init_stack(stack);
	BiTree p = T;	/* traverse pointer */

	while (p || !stack_empty(stack)) {
		if (p) {
			push(stack, p);
			p = p->lchild;
		}
		else {
			pop(stack, &p);
			visit(p);
			p = p->rchild;
		}
	}
}

void post_order_2(BiTree T)
{
	a_stack *stack = malloc(sizeof(a_stack));
	init_stack(stack);
	BiTree p = T;
	BiTree r = NULL;	/* pointer to check */

	while (p || !stack_empty(stack)) {
		if (p) {
			push(stack, p);
			p = p->lchild;
		}
		else {
			get_top(stack, &p);	/* if rchild hasn't been visited, don't need to pop */
			if (p->rchild && r != p->rchild) 
				p = p->rchild;	/* next loop will push this into stack */
			else {
				pop(stack, &p);
				visit(p);
				r = p;
				p = NULL;	
			}
		}
	}
}

/* can give the right output, but will destroy original tree structure */
void post_order_destory(BiTree T)
{
	a_stack *stack = malloc(sizeof(a_stack));
	init_stack(stack);
	BiTree p = T;

	while (p || !stack_empty(stack)) {
		if (p) {
			push(stack, p);
			p = p->lchild;
		}
		else {
			pop(stack, &p);
			if (p->rchild) {	/* if has right child, push itself and rchild */
				push(stack, p);
				BiTree temp = p->rchild;
				p->rchild = NULL;	/* this destory original tree! */
				p = temp;
			}
			else {
				visit(p);
				p = NULL;	/* set p to NULL */
			}
		}
	}
}

int main(int argc, char *argv[])
{
	/* initialize tree structure */
	BiTNode T;
	BiTNode left_1, right_1, right_2, right_3, left_2;
	T 		= (BiTNode) { 1, &left_1, &right_1 };
	left_1 	= (BiTNode) { 2, NULL, &right_2 };
	right_1 = (BiTNode) { 3, NULL, &right_3};
	right_2 = (BiTNode) { 4, &left_2, NULL };
	left_2 	= (BiTNode) { 6, NULL, NULL };
	right_3 = (BiTNode) { 5,  NULL, NULL };

	BiTNode T_full;
	BiTNode l1, r1, l2, r2;
	T_full  = (BiTNode) { 1, &l1, &r1 };
	l1 		= (BiTNode) { 2, &l2, &r2 };
	r1 		= (BiTNode) { 3, NULL, NULL };
	l2 		= (BiTNode) { 4, NULL, NULL };
	r2 		= (BiTNode) { 5, NULL, NULL };

	BiTree root = &T;
	BiTree root_full = &T_full;

	post_order_2(root_full);
	printf("\n");
	post_order(root_full);
	printf("\n");

	return 0;
}
