#include<stdio.h>
#include<stdlib.h>
#include "list.h"

/* function prototypes */
fileList* SortedMerge(fileList* a, fileList* b);
void FrontBackSplit(fileList* source,
		fileList** frontRef, fileList** backRef);

/* sorts the linked list by changing next pointers (not data) */
void MergeSort(fileList** headRef)
{
	fileList* head = *headRef;
	fileList* a;
	fileList* b;

	/* Base case -- length 0 or 1 */
	if ((head == NULL) || (head->next == NULL))
	{
		return;
	}

	/* Split head into 'a' and 'b' sublists */
	FrontBackSplit(head, &a, &b);

	/* Recursively sort the sublists */
	MergeSort(&a);
	MergeSort(&b);

	/* answer = merge the two sorted lists together */
	*headRef = SortedMerge(a, b);
}

fileList* SortedMerge(fileList* a, fileList* b)
{
	fileList* result = NULL;

	/* Base cases */
	if (a == NULL)
		return(b);
	else if (b==NULL)
		return(a);

	/* Pick either a or b, and recur */
	if (a->numOccursInFile >= b->numOccursInFile)
	{
		result = a;
		result->next = SortedMerge(a->next, b);
	}
	else
	{
		result = b;
		result->next = SortedMerge(a, b->next);
	}
	return(result);
}

/* UTILITY FUNCTIONS */
/* Split the nodes of the given list into front and back halves,
   and return the two lists using the reference parameters.
   If the length is odd, the extra node should go in the front list.
   Uses the fast/slow pointer strategy.  */
void FrontBackSplit(fileList* source,
		fileList** frontRef, fileList** backRef)
{
	fileList* fast;
	fileList* slow;
	if (source==NULL || source->next==NULL)
	{
		/* length < 2 cases */
		*frontRef = source;
		*backRef = NULL;
	}
	else
	{
		slow = source;
		fast = source->next;

		/* Advance 'fast' two nodes, and advance 'slow' one node */
		while (fast != NULL)
		{
			fast = fast->next;
			if (fast != NULL)
			{
				slow = slow->next;
				fast = fast->next;
			}
		}

		/* 'slow' is before the midpoint in the list, so split it in two
		   at that point. */
		*frontRef = source;
		*backRef = slow->next;
		slow->next = NULL;
	}
}

/* Function to print nodes in a given linked list */
void printList(fileList *node)
{
	while(node!=NULL)
	{
		printf("%d ", node->numOccursInFile);
		node = node->next;
	}
}

/* Function to insert a node at the beginging of the linked list */
void push(fileList** head_ref, int new_data)
{
	/* allocate node */
	fileList* new_node = (fileList*) malloc(sizeof(fileList));

	/* put in the data  */
	new_node->numOccursInFile  = new_data;

	/* link the old list off the new node */
	new_node->next = (*head_ref);   

	/* move the head to point to the new node */
	(*head_ref)    = new_node;
}
int freeList(fileList *destroy){
	fileList *tmp;
	while(destroy->next != NULL){
		tmp = destroy->next;
		free(destroy);
		destroy = tmp;
	}
	free(destroy);
}
/* Drier program to test above functions*/
/*
int main()
{
	
	fileList* res = NULL;
	fileList* a = NULL;

	
	push(&a, 15);
	push(&a, 10);
	push(&a, 5);
	push(&a, 20);
	push(&a, 3);
	push(&a, 2);
	
	
	MergeSort(&a);

	printf("\n Sorted Linked List is: \n");
	printList(a);          
	freeList(a);
	getchar();
	return 0;
}
*/
