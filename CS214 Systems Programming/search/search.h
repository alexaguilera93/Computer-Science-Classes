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

void addWord(char *word);

void addFile(char *word, char *file);

int isalpha(int c);

int tolower(int c);

void destroyNodes();

void destroyFiles(fileList *fileHead);

int sendString(char *inputString);

char *getFileString(char *fileName);

fileList* logAndList(fileList *listOne, fileList *listTwo);

fileList* logOrList(fileList *listOne, fileList *listTwo);

#endif
