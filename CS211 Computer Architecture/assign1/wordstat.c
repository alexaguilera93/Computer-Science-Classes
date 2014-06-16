db#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <malloc.h>
#include <string.h>
#include "wordstat.h"
NODE *Root;
void processStr(char* str);
void traverseprint(NODE *N);
void printResult();
//read arguement from command line
int main(int argc, char **argv){
if(argc != 2){
printf("please rerun program and enter filename\n");
return 0;
}
if(strcmp(argv[1], "-h") == 0){
FILE *qp;
qp = fopen("readme.pdf", "r");
char ch;
while((ch=fgetc(qp))!=EOF){
printf("%c", ch);

}
return 0;
}
FILE *fp;
fp = fopen(argv[1],"r");
if (fp == NULL){
printf("Error, file in argument could not be found");
return 1;
}
char ch;
int check = 0;
int resize = 30;
char* currWord;
currWord = malloc(resize * sizeof(char));
//read through file one char at a time and add to string until word is complete then 
//move to next word until file is completed
while((ch=fgetc(fp))!=EOF){
	char currChar = ch;

	if(!(isalpha(currChar)) && check == 0){
		continue;
	}


	if((isalpha(currChar)) && check == 0){
		currWord[check] = ch;
		check = 1;
		continue;
	}	
	if(((isalpha(currChar)) || (isdigit(currChar))) && check > 0){
		if(check +1 >= resize){
			resize = resize * 2;
			char *temp = malloc(resize * sizeof(char));
			strcpy(temp, currWord);
			currWord = malloc(resize * sizeof(char));
			strcpy(currWord, temp);
			currWord[check] = ch;
			free(temp);
			check = check + 1;
			continue;
		}
		currWord[check] = ch;
	check = check + 1;
	continue;


	}
	if(!(isalpha(currChar)) && check != 0){
		processStr(currWord);
		resize = 30;
		check = 0;
		free(currWord);
		currWord = malloc(resize * sizeof(char));
		continue;


	}
	}
	printResult();	
	return 0;
}

//add occurance of the string to tree.
void processStr(char* str){
	char* lowerVersion = malloc(strlen(str) * sizeof(char));
	char* originalVersion = malloc(strlen(str) * sizeof(char));
	for(int i = 0; i < strlen(str); i++){
	originalVersion[i] = str[i];
	lowerVersion[i] = tolower(str[i]);
	}
	
	
	NODE* newNode;
	newNode = malloc(sizeof(NODE));
	newNode = createNode(originalVersion, lowerVersion);
	add(newNode, Root);
	}






void printResult(){
printf("Word\t"  "Total No. Occurances\t"  "No. Case-Sensitive Versions\n");
traverseprint(Root);

}
//traverse tree recursively and print in order
void traverseprint(NODE* N){
if(N == NULL){
	return;
}

traverseprint(N->leftchild);
	
	int caseoccur = 1;
	printf("%s\t%d\t",N->wordwocase, N->numOfOccur);
	NODE* temp1;
	temp1 = N;
	while(temp1->sibling != NULL){
	caseoccur = caseoccur +1;
	temp1 = temp1->sibling;
	}	
	printf("%d\n", caseoccur);
traverseprint(N->rightchild);
free(N);
}

//create a new node to be added to tree
NODE* createNode(char *wordwcase, char *wordwocase){
NODE *newNode1;
newNode1 = malloc(sizeof(NODE));
newNode1->wordwcase = malloc(strlen(wordwcase)* sizeof(char));
newNode1->wordwocase= malloc(strlen(wordwocase)* sizeof(char));
newNode1->wordwcase = wordwcase;
newNode1->wordwocase = wordwocase;
newNode1->numOfOccur = 1;
newNode1->numOfCaseOccur = 1; 
return newNode1;
}
//add new node or occurance of word to tree
void add(NODE *nodeToAdd, NODE *tree){
	if(Root == NULL){
	Root = nodeToAdd;
	return;
	}
	int strcomparison = strcmp(nodeToAdd->wordwocase, tree->wordwocase);
	if(strcomparison == 0){
	tree->numOfOccur = tree->numOfOccur + 1;
	nodeToAdd->numOfOccur = nodeToAdd->numOfOccur + 1;
	
	int strcomparison2 = strcmp(nodeToAdd->wordwcase , tree->wordwcase);
		if(strcomparison2 == 0){
			tree->numOfCaseOccur = tree->numOfCaseOccur + 1;
			free(nodeToAdd);
			return;
		}
	NODE* temp;
	temp = tree;	
	while(temp->sibling != NULL){
		temp = temp->sibling;
		temp->numOfOccur = temp->numOfOccur + 1;
		nodeToAdd->numOfOccur = nodeToAdd->numOfOccur + 1;
		strcomparison2 = strcmp(nodeToAdd->wordwcase, temp->wordwcase);
			if(strcomparison2 == 0){
				temp->numOfCaseOccur = temp->numOfCaseOccur + 1;
				free(nodeToAdd);
				return;
			}
		}
	
	temp->sibling = nodeToAdd;
	return;


	}
	if(strcomparison > 0){
		if(tree->rightchild == NULL){


		tree->rightchild = nodeToAdd;
		return;	
		}
	add(nodeToAdd,tree->rightchild); 
	}

	if(strcomparison < 0){
		if(tree->leftchild == NULL){
		tree->leftchild = nodeToAdd;
		return;
		}

	add(nodeToAdd,tree->leftchild);
	}




}


