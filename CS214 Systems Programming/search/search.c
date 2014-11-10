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
andLoop myCheck = KEEPGOING;

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
		if(string1 == NULL){
		printf("Empty/corrupt file\n");
		return 1;
		}
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
		printf("Usage: for AND sa <term> <term> .... for OR so <term> <term> ..... to quit type q:\n");
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
		//do or of lists
		if(strcmp("so", token) == 0){
			free(token);
			fileList *hold = NULL;
			while((token = TKGetNextToken(tokenizer)) != NULL){
				Node *trace1 = head;
				//head node word is the token
				if(strcmp(trace1->word, token) == 0){
					if(hold == NULL){
						hold = makeList(trace1->listHead);
						free(token);
						continue;
					}
					else{
						fileList *newHold = logOrList(hold, trace1->listHead);
						destroyFiles(hold);
						free(token);
						hold = newHold;
						continue;
					}
				}
				else{
					while(trace1->next != NULL){
						trace1 = trace1->next;
						if(strcmp(trace1->word, token) == 0){
							if(hold == NULL){
								hold = makeList(trace1->listHead);
								//free(token);
								break;
							}
							else{
								fileList *newHold1 = logOrList(hold, trace1->listHead);
								destroyFiles(hold);
								hold = newHold1;
								//free(token);
								break;
							}
						}
					}
					free(token);
				}

			}
			free(token);
			TKDestroy(tokenizer);
			printList(hold);
			destroyFiles(hold);
			return 0;
		}

		else if(strcmp("sa", token) == 0){
				free(token);
                fileList *hold = NULL;
		while((token = TKGetNextToken(tokenizer)) != NULL){
			Node *trace2 = head;
			if(strcmp(trace2->word, token) == 0){
				if(hold == NULL){
					hold = makeList(trace2->listHead);
					free(token);
					continue;
				}
				else{
					fileList *newHold = logAndList(hold, trace2->listHead);
					destroyFiles(hold);
					free(token);
					hold = newHold;

				}
			}
			else{
				//0 false 1 true
			//int reachEnd = 0;
			while(trace2->next != NULL){
				trace2 = trace2->next;
				if(strcmp(trace2->word, token) == 0){
					if(hold == NULL){
						hold = makeList(trace2->listHead);
						break;
					}
					else{
					fileList *newHold1 = logAndList(hold, trace2->listHead);
					destroyFiles(hold);
					hold = newHold1;
					if(myCheck == STOP){
						printf("Query returned no results\n");
						while((token = TKGetNextToken(tokenizer)) != NULL){
							free(token);
						}
						TKDestroy(tokenizer);
						myCheck = KEEPGOING;
						return 0;
				}
					}
					break;
				}
			}
			if(trace2->next == NULL){
				//printf("Query returned no results \n");
				destroyFiles(hold);
				myCheck = STOP;
				hold = NULL;
			}

			}
			free(token);
			token = NULL;
		if(myCheck == STOP){
				myCheck = KEEPGOING;
				break;
			}	
		}
		if(token != NULL){
			free(token);
		}
		while((token = TKGetNextToken(tokenizer)) != NULL){
		free(token);
		}

		TKDestroy(tokenizer);
		printList(hold);
		destroyFiles(hold);
		}
		else{
			printf("Invalid Input\n");
		}
		return 0;
	}
	return 0;
}

void printList(fileList *print){
	if(print == NULL){
		printf("Query returned no results\n");
	}
	else{
		printf("%s ", print->file);
		while(print->next != NULL){
			print = print->next;
			printf("%s ", print->file);
		}

	}
	printf("\n");
}
fileList *makeList(fileList *copy){
	fileList *newFileHead;
	newFileHead = (fileList*)malloc(sizeof(fileList));
	memset(newFileHead, 0, sizeof(fileList));
	char *newString;
	int len = strlen(copy->file);
	newString = (char*)malloc(len + 1);
	strcpy(newString, copy->file);
	newString[len] = '\0';
	newFileHead->file = newString;
	fileList *back;
	back = newFileHead;
	while(copy->next != NULL){
		copy = copy->next;
		fileList *newFile;
		newFile = (fileList*)malloc(sizeof(fileList));
		memset(newFile, 0, sizeof(fileList));
		char *newStr;
		int len1 = strlen(copy->file);
		newStr = (char*)malloc(len1 + 1);
		strcpy(newStr, copy->file);
		newStr[len1] = '\0';
		newFile->file = newStr;
		back->next = newFile;
		back = back->next;
	}
	return newFileHead;

}

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
	if(fileHead == NULL){
		return;
	}
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
	newFile = (char*)malloc(fileLen + 1);
	memset(newFile, 0, fileLen + 1);
	strcpy(newFile, file);
	newFile[fileLen] = '\0';
	fileList *newListIt;
	newListIt = (fileList*)malloc(sizeof(fileList));
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
	fileList *newList = NULL;
	fileList *trace;
	if(listOne == NULL || listTwo == NULL){
		return NULL;
	}
	trace = listOne;

	if(inList(trace->file, listTwo) == 1){
		newList = andAddToList(trace->file, newList);
	}
	while(trace->next != NULL){
		trace = trace->next;
		if(inList(trace->file, listTwo) == 1){
			newList = andAddToList(trace->file, newList);
		}
	}
	if(newList == NULL){
		myCheck = STOP;
	}
	return newList;
}

//perform a logical OR on 2 lists, returns head of new list that is logical OR of nodes in listOne and listTwo
fileList* logOrList(fileList *listOne, fileList *listTwo){
	fileList *newList = makeList(listOne);
	//not in list
	if(inList(listTwo->file, newList) == 0){
		addToList(listTwo->file, newList); 
	}

	while(listTwo->next != NULL){
		listTwo = listTwo->next;
		if(inList(listTwo->file, newList) == 0){
			addToList(listTwo->file, newList); 
		}

	}
	return newList;
}

fileList *andAddToList(char *toBeAdded, fileList *addedTo){
	fileList *head = addedTo;
	char *newStr;
	int len;
	len = strlen(toBeAdded);
	newStr = (char*)malloc(len + 1);
	strcpy(newStr, toBeAdded);
	newStr[len] = '\0';
	fileList *new = (fileList*)malloc(sizeof(fileList));
	memset(new, 0, sizeof(fileList));
	new->file = newStr;
	if(addedTo == NULL){
		addedTo = new;
		addedTo->next = NULL;
		return addedTo;
	}
	else{
	while(addedTo->next != NULL){
		addedTo = addedTo->next;
	}
	addedTo->next = new;
	return head;
	}

}
void addToList(char *toBeAdded, fileList *addedTo){
	char *newStr;
	int len;
	len = strlen(toBeAdded);
	newStr = (char*)malloc(len + 1);
	strcpy(newStr, toBeAdded);
	newStr[len] = '\0';
	fileList *new = (fileList*)malloc(sizeof(fileList));
	memset(new, 0, sizeof(fileList));
	new->file = newStr;
	if(addedTo == NULL){
		addedTo = new;
		addedTo->next = NULL;
		return;
	}
	else{
	while(addedTo->next != NULL){
		addedTo = addedTo->next;
	}
	addedTo->next = new;
	}
}
//check if file is in list return 1 for yes 0 for no
int inList(char *check, fileList *listH){
	if(strcmp(listH->file, check) == 0){
		return 1;
	}
	else{
		while(listH->next != NULL){
			listH = listH->next;
			if(strcmp(listH->file, check) == 0){
				return 1;
			}
		}
	}
	return 0;
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
