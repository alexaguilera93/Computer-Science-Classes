#include "book.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <string.h>
#include "tokenizer.h"
#include <pthread.h>
#include <semaphore.h>

#define FALSE 0
#define TRUE 1
#define SBUFSIZE 16 

//initialize SharedData
char **process_catagories(char *fileName){
	FILE *fp;
	fp = fopen(fileName, "r");
	char ch;
	struct stat st;
	if(stat(fileName, &st) != 0){
		fclose(fp);
		return NULL;
	}
	int sizeCheck = st.st_size;
	if(sizeCheck == 0){
		fclose(fp);
		return NULL;
	}
	else{
		char *fileString;
		fileString = (char*)malloc(sizeof(char) * sizeCheck + 1);
		memset(fileString, 0, sizeCheck + 1);
		int count = 0;
		int wordcount = 0;
		while((ch=fgetc(fp)) != EOF){
			if(ch == ' ' || ch == '\n'){
			wordcount++;
			}
			fileString[count] = ch;
			count++;
		}
		fileString[sizeCheck] = '\0';
		char **TBR;
		TBR = (char**)malloc(sizeof(char*) * wordcount);
		TokenizerT tokenizer = TKCreate(" \n", fileString);
		char *token;
	        while((token = TKGetNextToken(tokenizer)) != NULL){
			
	        }

	}
}
void consumer_func(void *arg){



}

void producer_func(void *arg){


}
