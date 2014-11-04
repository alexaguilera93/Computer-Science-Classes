#ifndef LIST_H
#define LIST_H
struct Node_{
	char* word;
	struct fileList_ *listHead;
	struct Node_ *next;
	int isHead;
};
typedef struct Node_ Node;


struct fileList_ {
	char* file;
	int numOccursInFile;
	struct fileList_ *next;
};
typedef struct fileList_ fileList;

void destroyList(fileList *destroy);

void MergeSort(fileList** headRef);

int insert(char *wordToAdd, char *File);

int addOccur(Node *addedTo, char *File);

Node *createNode(char *wordToAdd, char *File);

void destroyNodes();

void printNodes(char *fileName);

#endif
