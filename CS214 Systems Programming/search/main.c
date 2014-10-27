#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include "tokenizer.h"
#include "list.h"

char *getFileString(char *filename);
int openDirRec(char* toBeOpened);
int isalpha(int c);
int tolower(int c);
int processFile(char *fileName);
void printNodes();
void destroyNodes();
char *makeDirName(const char* one, const char* two);
int main(int argc, char** argv){
	if(argc != 3){
		printf("not valid # of arguments");
		return 1;
	}
	struct stat info;
	struct stat st;
	int status;
	status = stat(argv[2], &info);
	if(stat(argv[1], &st) == 0){
	int yn;
	printf("file already exists would you like to overwrite it? 1 for yes 0 for no\n");
	
	scanf("%d", &yn);
		
		if(yn == 0){
		return 1;
		}
	}

	if(status != 0){
		printf("Error, errno = %d\n", errno);
		return 1;
	}
	//command line argument is file
	if(S_ISREG (info.st_mode)){
		char *string1;
		string1 = getFileString(argv[2]);
		if(string1 == NULL){
		printf("No text in file");
		return 1;
		}
		TokenizerT* tokenizer = TKCreate(" ", string1);
		char *token;
		while((token = TKGetNextToken(tokenizer)) != NULL){
			insert(token, argv[2]);
			free(token);
		}
		free(string1);
		TKDestroy(tokenizer);
		printNodes(argv[1]);
		destroyNodes();
		return 0;
	}
	if(S_ISDIR(info.st_mode)){	
		//printf("%s is a directory \n", argv[2]);
		openDirRec(argv[2]);
		printNodes(argv[1]);
		destroyNodes();
		//what to do if command line argument is directory
	}
	return 0;
}

int processFile(char* fileName){
	char *stringa;
	stringa = getFileString(fileName);
	if(stringa == NULL){
	free(fileName);
	return 1;
	}
	TokenizerT* tokenizer = TKCreate(" ", stringa);
	char *token;
	while((token = TKGetNextToken(tokenizer)) != NULL){
		insert(token, fileName);
		free(token);
	}
	free(stringa);
	free(fileName);
	TKDestroy(tokenizer);
	return 0;
}

char* getFileString(char *fileName){
	FILE* qp;
	qp = fopen(fileName, "r");
	char ch;
	struct stat st;
	if(stat(fileName, &st) != 0) {
		fclose(qp);
		return NULL;
	}
	/*int sizeCheck = 0;
	  while((ch=fgetc(qp))!=EOF){
	  sizeCheck++;
	  }
	  */
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
//read through directories when user inputs a directory
int openDirRec(char* toBeOpened){
	//get directory to open
	DIR *d;
	d = opendir(toBeOpened);
	//can't open, print error
	if(!d){
		printf("directory open failed");
		return 0;
	}
	//get all elements in directory
	while(1){
		//struct dirent is an entry inside of a directory
		struct dirent *entry;
		const char *d_name;
		//get next entry in directory if none break loop
		entry = readdir(d);
		if(! entry){
			//no entries;
			break;
		}
		d_name = entry->d_name;
		//printf("%s/%s\n", toBeOpened, d_name);
		//for when entry is a directory, if check and make sure its a child directory and 
		//call openDirRec recusively
		if(entry->d_type & DT_DIR) {
			if (strcmp (d_name, "..") != 0 && strcmp (d_name, ".") != 0){
				int pathlen;
				char path[PATH_MAX];
				pathlen = snprintf(path, PATH_MAX, "%s/%s", toBeOpened, d_name);
				//printf("%s\n", path);
				if(pathlen >= PATH_MAX){
					fprintf(stderr, "Path length is too long");
				}
				openDirRec(path);
			}
		}
		else if(entry->d_type & DT_REG){
			char *getFile = makeDirName(toBeOpened, d_name);
			processFile(getFile);
			
		}
	}
	//close directory
	if(closedir(d)){
		fprintf(stderr, "could not close '%s':%s\n", toBeOpened, strerror(errno));
	}
	
	return 0;

}

char *makeDirName(const char* one, const char* two){
	char *newName;
	int str1len;
	int str2len;
	str1len = strlen(one);
	str2len = strlen(two);
	newName = (char*) malloc(str1len + str2len + 2);
	int i;
	for(i = 0; i < str1len; i++){
		newName[i] = one[i];
	}
	newName[str1len] = '/';
	int j = str1len + 1;
	for(i = 0; i < str2len; i++){
		newName[j] = two[i];
		j++;
	}
	newName[str2len + str1len + 1] = '\0';
	return newName;
}
