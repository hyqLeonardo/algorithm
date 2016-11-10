#include <stdio.h>

#include "stack.h"

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
	if (stack->top == MAXSIZE-1)	/* stack is full */
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
