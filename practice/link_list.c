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

/* split a linked list into one odd indexed list and one even indexed list */
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
	single_llh *even = split_llh(head);
	print_llh(head);
	print_llh(even);
}

