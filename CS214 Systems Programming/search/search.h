#ifndef SEARCH_H
#define SEARCH_H

//Node to hold words
struct Node_{
	char* word;
	struct fileList_ *listHead;
	struct Node_ *next;
	int isHead;
};
typedef struct Node_ Node;

//fileList Node
struct fileList_ {
	char* file;
	int numOccursInFile;
	struct fileList_ *next;
};
typedef struct fileList_ fileList;

typedef enum {KEEPGOING, STOP} andLoop;

void addWord(char *word);

fileList *andAddToList(char *toBeAdded, fileList *addedTo);

void addFile(char *word, char *file);

int isalpha(int c);

int tolower(int c);

int inList(char *check, fileList *listH);

void printList(fileList *print);

fileList *makeList(fileList *copy);

void destroyNodes();

void destroyFiles(fileList *fileHead);

int sendString(char *inputString);

char *getFileString(char *fileName);

fileList* logAndList(fileList *listOne, fileList *listTwo);

fileList* logOrList(fileList *listOne, fileList *listTwo);

void addToList(char *toBeAdded, fileList *addedTo);
#endif
