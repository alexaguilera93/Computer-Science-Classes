#include<stdio.h>
#include<stdlib.h>
#include "list.h"
#include "mergesort.h"


void MergeSort(fileList** headRef)
{
	fileList* head = *headRef;
	fileList* a;
	fileList* b;

	// Base case length 0 or 1 
	if ((head == NULL) || (head->next == NULL))
	{
		return;
	}

	FrontBackSplit(head, &a, &b);

	//Recursively sort the sublists
	MergeSort(&a);
	MergeSort(&b);

	*headRef = SortedMerge(a, b);
}

fileList* SortedMerge(fileList* a, fileList* b)
{
	fileList* result = NULL;

	// Base cases 
	if (a == NULL)
		return(b);
	else if (b==NULL)
		return(a);

	// Pick a or b, and recurse
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


//Split the nodes of the given list into front and back
void FrontBackSplit(fileList* source,
		fileList** frontRef, fileList** backRef)
{
	fileList* fast;
	fileList* slow;
	if (source==NULL || source->next==NULL)
	{
		
		*frontRef = source;
		*backRef = NULL;
	}
	else
	{
		slow = source;
		fast = source->next;
		while (fast != NULL)
		{
			fast = fast->next;
			if (fast != NULL)
			{
				slow = slow->next;
				fast = fast->next;
			}
		}
		*frontRef = source;
		*backRef = slow->next;
		slow->next = NULL;
	}
}

/*
void printList(fileList *node)
{
	while(node!=NULL)
	{
		printf("%d ", node->numOccursInFile);
		node = node->next;
	}
}
*/
/*
int freeList(fileList *destroy){
	fileList *tmp;
	while(destroy->next != NULL){
		tmp = destroy->next;
		free(destroy);
		destroy = tmp;
	}
	free(destroy);
}
*/
