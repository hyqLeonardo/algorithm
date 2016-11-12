// #pragma once
/* 
 * stack implementation
 */

#include <stdio.h>
#include <stdlib.h>

#include "bst.h"

#define MAXSIZE 50
#define TRUE 1
#define FALSE 0

/* type of elements in bst */
typedef BiTree stack_elem_t;

/*
 * struct definition of array based stack
 */
typedef struct a_stack {
	stack_elem_t data[MAXSIZE];
	int top;	/* position of top element */
} a_stack;

/* not implemented */
typedef struct l_stack {
	stack_elem_t data;
	struct l_stack *next;
} l_stack;

void init_stack(a_stack *stack);
int stack_empty(a_stack *stack);
int push(a_stack *stack, stack_elem_t a);
int pop(a_stack *stack, stack_elem_t *x);
int get_top(a_stack *stack, stack_elem_t *x);
