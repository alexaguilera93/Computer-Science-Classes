#include "list.h"
#include "tokenizer.h"
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

Node *head;
//making linked list for the words and files
int insert(char *wordToAdd2, char *File){
	char *wordToAdd;
	int length5 = strlen(wordToAdd2);
	wordToAdd = (char*) malloc(length5 + 1);
	strcpy(wordToAdd, wordToAdd2);
	if(head == NULL){
		Node *new;
		new = createNode(wordToAdd, File);
		//free(File);
		new->isHead = 1;
		head = new;
		return 0;
	}
	else{
		Node *trace;
		trace = head;
		char *traceWord;
		int wordSize;
		wordSize = strlen(trace->word);
		traceWord = (char*) malloc(wordSize + 1);
		strcpy(traceWord, trace->word);
		int a =strcmp(wordToAdd, traceWord);
		free(traceWord);
		if(a == 0){
			addOccur(trace, File);
			//printf("addOccur returned %d\n", b);
			free(wordToAdd);
			return 0;
		}
		if(a < 0){
			Node *Insert = createNode(wordToAdd, File);
			trace->isHead = 0;
			Insert->isHead = 1;
			Insert->next = trace;
			head = Insert;
			return 0;
		}
		else{	

			Node *backTrace;
			backTrace = head;

			while(trace->next != NULL){
				trace = trace->next;
				traceWord = trace->word;
				a = strcmp(wordToAdd, traceWord);
				if(a < 0){
					Node* Insert = createNode(wordToAdd, File);
					Insert->next = trace;
					backTrace->next = Insert;
					return 0;
				}
				if(a == 0){
					addOccur(trace, File);
					free(wordToAdd);
					return 0;
				}
				if(a > 0){
					backTrace = trace;
					continue;
				}
			}
			Node *Insert = createNode(wordToAdd, File);
			Insert->next = NULL;
			trace->next = Insert;
			return 0;
		}
	}
	return 1;
}

//create new occurance of a newe word with file
Node* createNode(char *wordToAdd, char *File){
	Node *new;
	new = (Node*)malloc(sizeof(Node));
	memset(new, 0, sizeof(Node));
	new->word = wordToAdd;
	fileList *startList;
	startList = (fileList*)malloc(sizeof(fileList));
	memset(startList, 0, sizeof(fileList));
	char *file2;
	int fileStrLen = strlen(File);
	file2 = (char*) malloc(fileStrLen + 1);
	strcpy(file2, File);
	file2[fileStrLen] = '\0';
	//free(File);
	startList->file = file2;
	startList->numOccursInFile = 1;
	new->listHead = startList;
	return new;
}
//add occurance to an already existing word
int addOccur(Node *addedTo, char *File){
	fileList *check = addedTo->listHead;
	char *fileCheck;
	while(check->next != NULL){
		fileCheck = check->file;
		int a = strcmp(fileCheck, File);
		if(a == 0){
			int j = check->numOccursInFile;
			j++;
			check->numOccursInFile = j;
			//free(File);
			return 0;
		}
		check = check->next;
	}
	fileCheck = check->file;
	int c = strcmp(fileCheck, File);
	if(c ==0){
		int l = check->numOccursInFile;
		l++;
		check->numOccursInFile = l;
		//free(File);
		return 0;
	}
	else{
		fileList *startList;
		startList = (fileList*)malloc(sizeof(fileList));
		memset(startList, 0, sizeof(fileList));
		char *newFile1;
		int nfonelen = strlen(File);
		newFile1 = (char*) malloc(nfonelen + 1);
		strcpy(newFile1, File);
		newFile1[nfonelen] = '\0';
		startList->file = newFile1;
		startList->numOccursInFile = 1;
		//free(File);
		check->next = startList;
		return 0;
	}
	return 1;
}
//print out the nodes for info
void printNodes(char *fileName){
	if(head == NULL){
	printf("empty directory/file, nothing to write\n");
	return;
	}
	Node *trace;
	fileList *tra2;
	FILE *writeFile;
	writeFile = fopen(fileName, "w");
	trace = head;
	while(trace->next != NULL){
		fileList *tmp5;
		tmp5 = trace->listHead;
		MergeSort(&tmp5);
		trace->listHead = tmp5;
		fprintf(writeFile, "<list> %s\n", trace->word);
		tra2 = trace->listHead;
		while(tra2->next != NULL){
			fprintf(writeFile, "%s %d ", tra2->file, tra2->numOccursInFile);
			tra2 = tra2->next;
		}
		fprintf(writeFile,"%s %d \n</list> \n", tra2->file, tra2->numOccursInFile);
		trace = trace->next;
	}
	fprintf(writeFile, "<list> %s\n", trace->word);
	tra2 = trace->listHead;
	while(tra2->next != NULL){
		fprintf(writeFile, "%s %d ", tra2->file, tra2->numOccursInFile);
		tra2 = tra2->next;
	}
	fprintf(writeFile, "%s %d\n</list>\n", tra2->file, tra2->numOccursInFile);
fclose(writeFile);
}

/*
   int compareFunct(char *str1, char* *str2){
   return 0;
   }
   */
void destroyNodes(){
	Node *destruct;
	if(head == NULL){
	return;
	}
	destruct = head;
	Node *destruct2;
	char *destWord;
	while(destruct->next != NULL){
		fileList *destruct3;
		destruct3 = destruct->listHead;
		destroyList(destruct3);
		destruct2 = destruct->next;
		destWord = destruct->word;
		if(destWord != NULL){
			free(destruct->word);
		}
		free(destruct);
		destruct = destruct2;
	}
	fileList *destruct4;
	free(destruct->word);
	destruct4 = destruct->listHead;
	destroyList(destruct4);
	free(destruct);
}


void destroyList(fileList *destroy){
	fileList *trace;
	while(destroy->next != NULL){
		trace = destroy->next;
		char *destFile;
		destFile = destroy->file;
		free(destFile);
		free(destroy);
		destroy = trace;
	}
	char *dest2;
	dest2 = destroy->file;
	if(dest2 != NULL){
		free(dest2);
	}
	free(destroy);
}
/*
   int main(){
   int l = insert("Hi", "File2.txt");
   int a = insert("Hi", "File1.txt");
   int b = insert("Ab", "File1.txt");
   int c = insert("Aa", "File1.txt");
   int d = insert("zzzzzzz", "File1.txt");
   int h = insert("Hi", "File2.txt");
   int i = insert("Hi", "File3.txt");
   int j = insert("Abc", "File2.txt");
   int k = insert("Tg", "File4.txt");
   int m = insert("Mq", "File8.txt");
   int n = insert("Tm", "File20.txt");
   int o = insert("Awb", "File99.txt");
   int p = insert("Ls", "File5.txt");
   int q = insert("Hi", "File2.txt");
   printf("insert returned %d %d %d %d %d %d %d %d \n", l, a, b, c, d, j, i, h);
   printNodes();
   destroyNodes();
   return 0;
   }
   */
