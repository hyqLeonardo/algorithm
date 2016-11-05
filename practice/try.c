#include <stdio.h>

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
	stack->top = -1;
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

/* traverse using stack structure */
void in_order_2(BiTree T)
{
	a_stack s = { .top = -1 };
	a_stack *stack = &s;
	// init_stack(stack);
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

int tell(int a)
{
	return a > 0 ? 1 : 0;
}

int main()
{
	BiTNode a;
	BiTNode b;
	BiTNode root;
	// BiTree p;
	a = (BiTNode) { 1, NULL, NULL };
	b = (BiTNode) { 2, NULL, NULL };
	root = (BiTNode) { 0, &a, &b };
	a_stack s = { .top = -1 };
	a_stack *stack = &s;

	// visit(&root);
	// visit(&a); 
	// visit(&b);

	// printf("size of &b is : %d bytes\n", (int)sizeof(&b));
	// // p = &b;
	// printf("size of p is : %d bytes\n", (int)sizeof(p));
	// // printf("p is %p\n", p);
	// printf("size of stack is : %d bytes\n", (int)sizeof(stack));
	// printf("number of elements of stack's array is : %d\n", 
	// 	(int)(sizeof(stack->data)/sizeof(BiTree)));
	// // init_stack(stack);
	// printf("stack top : %d\n", stack->top);
	// push(stack, &root);
	// printf("now, stack top is : %d\n", stack->top);
	// printf("stack top element has size : %d\n", (int)sizeof(stack->data[stack->top]));
	// printf("element data in top is : %d\n", stack->data[stack->top]->data);	
	// push(stack, &a);
	// push(stack, &b);
	// printf("now, stack top is : %d\n", stack->top);
	// printf("element data in top is : %d\n", stack->data[stack->top]->data);	
	// printf("address of b is %p\n", &b);
	// printf("element of stack top (which should be address of b is : %p\n", 
	// 	stack->data[stack->top]);
	// p = stack->data[stack->top];
	// visit(p);	
	// stack->top -= 1;
	// printf("now, stack top is : %d\n", stack->top);
	// pop(stack, &p);
	// visit(p);
	// pop(stack, &p);
	// visit(p);
	// pop(stack, &p);
	// visit(p);
	// push(stack, &root);
	// pop(stack, &p);
	// visit(p);

	// int count = 0;
	// while (pointer) {
	// 	count++;
	// 	push(stack, pointer);
	// 	pointer = pointer->lchild;
	// }
	// printf("%d push done\n", count);
	// count = 0;
	// while (!stack_empty(stack)) {
	// 	count++;
	// 	pop(stack, &p);
	// 	// printf("stack top : %d\n", stack->top);
	// 	visit(p);
	// }
	// printf("\n%d pop done\n", count);

	/* initialize tree structure */
	BiTNode T;
	BiTNode left_1;
	BiTNode right_1;
	BiTNode right_2;
	BiTNode right_3;
	BiTNode left_2;

	T 		= (BiTNode) { 1, &left_1, &right_1 };
	left_1 	= (BiTNode) { 2, NULL, &right_2 };
	right_1 = (BiTNode) { 3, NULL, &right_3};
	right_2 = (BiTNode) { 4, &left_2, NULL };
	left_2 	= (BiTNode) { 6, NULL, NULL };
	right_3 = (BiTNode) { 5,  NULL, NULL };

	BiTree pointer = &T;

	// while (pointer || !stack_empty(stack)) {
	// 	if (pointer) {
	// 		push(stack, pointer);
	// 		pointer = pointer->lchild;
	// 	}
	// 	else {
	// 		pop(stack, &pointer);
	// 		visit(pointer);
	// 		pointer = pointer->rchild;
	// 	}
	// }
	// printf("\n");

	in_order_2(pointer);

	return 0;
}
