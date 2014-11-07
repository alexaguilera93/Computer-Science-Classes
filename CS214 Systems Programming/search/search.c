#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include "stokenizer.h"
#include "search.h"

#define BUFFER_SIZE BUFSIZ
Node *head;
//main method for search
int main(int argc, char** argv){
	if(argc != 2){
		printf("not valid # of arguments\n");
		return 1;
	}
	struct stat info;
	struct stat st;

	int status;
	status = stat(argv[1], &info);
	if(stat(argv[1], &st) == 0){
		/*int yn;
		  printf("file already exists would you like to overwrite it? 1 for yes 0 for no\n");

		  scanf("%d", &yn);

		  if(yn == 0){
		  return 1;
		  }
		 */
	}
	if(status != 0){
		printf("Error, errno = %d\n", errno);
		return 1;
	}
	//command line argument is a file that exists
	if(S_ISREG(info.st_mode)){
		char *string1;
		string1 = getFileString(argv[1]);
		TokenizerT* tokenizer = TKCreate("  \n", string1);
		char *token;
		char *word;
		char *file;
		int j = 2;
		while((token = TKGetNextToken(tokenizer)) != NULL){
			if(strcmp(token, "list") == 0 || strcmp(token, "/list") == 0){
				word = TKGetNextToken(tokenizer);
				//add new word to memory
				addWord(word);
				//printf("%s", word);
				while(strcmp(file = TKGetNextToken(tokenizer), "list") != 0 && strcmp(file, "/list") != 0){
					if(j % 2 == 0){
						//printf("%s", file);
						//add file to word in memory
						addFile(word, file);
						free(file);
						j++;
					}
					else{
						//printf("%s", file);
						free(file);
						j++;
					}
				}
				free(word);
				free(file);
				free(token);
				j = 2;
			}
		}
		free(string1);
		char in[BUFFER_SIZE];
		int l;
		l = 0;
		if(fgets(in, BUFSIZ, stdin) != NULL){
			if(strlen(in) == 1){
				printf("Invalid Input\n");
			}
			else{
				l = sendString(in);
			}
		}
		else{
			printf("Invalid Input \n");
		}
		while(l == 0){
			if(fgets(in, BUFSIZ, stdin) != NULL){
				if(strlen(in) == 1){
					printf("Invalid Input\n");
				}
				else{
					l = sendString(in);
				}
			}
			else{
				printf("Invalid Input\n");
			}
		}
		destroyNodes();
		TKDestroy(tokenizer);
		return 0;
	}
	return 0;
}

int sendString(char *inputString){
	if(inputString == NULL){
		printf("No String\n");
		return 0;
	}
	if(!inputString){
		printf("No String\n");
		return 0;
	}
	if(strcmp(inputString, "q\n") == 0){
		return 1;
	}
	else{
		TokenizerT* tokenizer = TKCreate("  \n", inputString);
		char *token = TKGetNextToken(tokenizer);
		if(strcmp("so", token) != 0 && strcmp("sa", token) != 0){
			printf("Invalid Input\n");
			free(token);
			while((token = TKGetNextToken(tokenizer)) != NULL){
				free(token);
			}
			TKDestroy(tokenizer);
			return 0;
		}
		else{
			printf("User input was: %s\n", token);
			free(token);
			while((token = TKGetNextToken(tokenizer)) != NULL){
				free(token);
			}
			free(token);
			TKDestroy(tokenizer);
			return 0;
		}
		return 0;
	}
	return 0;
}
//command line arguemnt is directory
/*
   if(S_ISDIR(info.st_mode)){	
//printf("%s is a directory \n", argv[2]);
openDirRec(argv[2]);
printNodes(argv[1]);
destroyNodes();
//what to do if command line argument is directory
}
 */

//destroys linked list
void destroyNodes(){
	Node *trace;
	Node *back;
	trace = head;
	back = trace;
	fileList *fileTrace;
	while(trace->next != NULL){
		fileTrace = trace->listHead;
		free(trace->word);
		destroyFiles(fileTrace);
		back = trace;
		trace = trace->next;
		free(back);
	}
	fileTrace = trace->listHead;
	free(trace->word);
	destroyFiles(fileTrace);
	free(trace);	
}

//takes head of a list of file and frees all char* and fileLists
void destroyFiles(fileList *fileHead){
	fileList *backTrace;
	backTrace = fileHead;
	while(fileHead->next != NULL){
		backTrace = fileHead;
		free(fileHead->file);
		fileHead = fileHead->next;
		free(backTrace);
	}
	free(fileHead->file);
	free(fileHead);

}
//add file to already existing word
void addFile(char *word, char *file){
	char *newFile;
	int fileLen = strlen(file);
	newFile = malloc(fileLen + 1);
	memset(newFile, 0, fileLen + 1);
	strcpy(newFile, file);
	newFile[fileLen] = '\0';
	fileList *newListIt;
	newListIt = malloc(sizeof(fileList));
	memset(newListIt, 0, sizeof(fileList));
	newListIt->file = newFile;
	Node *trace;
	trace = head;
	if(strcmp(trace->word, word) == 0){
		if(trace->listHead == NULL){
			trace->listHead = newListIt;
			return;
		}
		else{
			fileList *trace1;
			trace1 = trace->listHead;
			while(trace1->next != NULL){
				trace1 = trace1->next;
			}
			trace1->next = newListIt;
			return;
		}
	}
	while(trace->next != NULL){
		trace = trace->next;
		if(strcmp(trace->word, word) == 0){
			if(trace->listHead == NULL){
				trace->listHead = newListIt;
				return;
			}
			else{
				fileList *trace2;
				trace2 = trace->listHead;
				while(trace2->next != NULL){
					trace2 = trace2->next;
				}
				trace2->next = newListIt;
				return;
			}
		}
	}

}

//add new word to list
void addWord(char *word){
	char *newWord;
	int wordLen;
	wordLen = strlen(word);
	newWord = malloc(wordLen + 1);
	memset(newWord, 0, wordLen + 1);
	strcpy(newWord, word);
	newWord[wordLen] = '\0';
	Node *newNode;
	newNode = malloc(sizeof(Node));
	memset(newNode, 0, sizeof(Node));
	newNode->word = newWord;
	if(head == NULL){
		head = newNode;
		return;
	}
	else{
		Node *trace;
		trace = head;
		while(trace->next != NULL){
			trace = trace->next;
		}
		trace->next = newNode;
		return;
	}
}
//perform a logical AND on 2 lists, returns head of new list that is logical AND of nodes in listOne and listTwo
fileList* logAndList(fileList *listOne, fileList *listTwo){

	return NULL;
}

//perform a logical OR on 2 lists, returns head of new list that is logical OR of nodes in listOne and listTwo
fileList* logOrList(fileList *listOne, fileList *listTwo){

	return NULL;
}

//get string in inverted index file
char* getFileString(char *fileName){
	FILE* qp;
	qp = fopen(fileName, "r");
	char ch;
	struct stat st;
	if(stat(fileName, &st) != 0) {
		fclose(qp);
		return NULL;
	}

	int sizeCheck = st.st_size;	
	if(sizeCheck == 0){
		fclose(qp);
		return NULL;
	}
	else{	
		//fseek(qp, SEEK_SET, 0);
		char *fileString;
		fileString = (char*)malloc(sizeof(char) * sizeCheck + 1);
		memset(fileString, 0, sizeCheck + 1);
		//rewind(qp);
		int count = 0;
		while((ch=fgetc(qp)) != EOF){	
			fileString[count] = ch;
			count++;
		}
		//printf("%s\n", fileString);
		fileString[sizeCheck] = '\0';
		int slength;
		slength = strlen(fileString);
		int l;

		for(l = 0; l < slength; l++){
			char ch;
			ch = fileString[l];
			if(isalpha(ch) && ch >= 'A' && ch <='Z'){
				char new;
				new = tolower(ch);
				fileString[l] = new;
			}
		}

		fclose(qp);
		return fileString;

	}
}
