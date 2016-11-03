/*
 * 2017 - data structure
 * p37 #11
 *
 */

#include <stdio.h>
#include <stdlib.h>

/* structure of single linked list with head node */
typedef struct single_llh {
	int data;
	struct single_llh *next;
} single_llh;

void print_llh(single_llh *head) {
	while (head->next != NULL) {
		head = head->next;
		printf("-> %d ", head->data);
	}
	printf("\n\n");
}

void insert_llh(single_llh *head, int value) {
	while (head->next != NULL) head = head->next;	/* now head is at the end */
	single_llh *new = (single_llh *)malloc(sizeof(single_llh));
	new->data = value;
	new->next = NULL;
	head->next = new;
}

/* #3 reversely print out data in linked list */
void rev_print_llh(single_llh *head) {
	if (head->next != NULL) {
		rev_print_llh(head->next);
	}
	printf("-> %d", head->data);
}

/* #6 convert a linked list into ascending order */
void ascend(single_llh *head) {
	/* empty list */
	if (head->next == NULL)
		return;
	single_llh *ascend;	/* ascending list */
	single_llh *pre;	/* previous node to current position in ascending list */
	single_llh *current = head->next;	/* point to next node to check in original list */
	single_llh *after = current->next;	/* node after current */
	head->next = NULL;	/* cut the ascending list */

	while (current != NULL) {
		ascend = head;	/* reinitialize ascending list head */
		pre = ascend;
		while (ascend != NULL) {
			if (current->data <= ascend->data) {
				/* insert current node in ascending list */
				current->next = pre->next;
				pre->next = current;
				/* update node to be checked one step forward */
				current = after;
				if (after != NULL) 
					after = after->next;
				break;
			}
			else {
				/* update pre and ascend pointer */
				pre = pre->next;
				ascend = ascend->next;
			}		
		}
		/* insert node at the end of ascending list */
		pre->next = current;
		current->next = NULL;	/* cut the ascending list */
		/* update node to be checked one step forward */
		print_llh(head);
		current = after;
		if (after != NULL)
			after = after->next;
	}
}

/* #11 split a linked list into one odd indexed list and one even indexed list */
single_llh *split_llh(single_llh *head) {
	single_llh *odd = head;	/* odd list */
	odd = odd->next;
	single_llh *even_head = (single_llh *)malloc(sizeof(single_llh));  /* even list */
	even_head->data = 0; even_head->next = NULL;
	single_llh *even = even_head;
	while (1) {
		even->next = odd->next;
		even = even->next;
		if (even->next == NULL) {
			odd->next = NULL;
			break;
		}
		odd->next = even->next;
		odd = odd->next;
	}
	return even_head;
}

int main(int argc, char *argv[])
{
	/* initialization */
	single_llh *head = (single_llh *)malloc(sizeof(single_llh));
	head->data = 0;
	head->next = NULL;
	single_llh *current = head;	
	int i;
	for (i = 1; i < argc; i++) {
		single_llh *new = (single_llh *)malloc(sizeof(single_llh));
		new->data = atoi(argv[i]);
		new->next = NULL;
		current->next = new;
		current = current->next;
	}

	/* #3 */
	// rev_print_llh(head);
	// printf("\n");

	/* #6 */
	print_llh(head);
	ascend(head);
	print_llh(head);

	/* #11 */
	// single_llh *even = split_llh(head);
	// print_llh(head);
	// print_llh(even);

	return 0;
}

