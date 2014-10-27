/*
 * tokenizer.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
/*
 * Tokenizer type.  You need to fill in the type as part of your implementation.
 */

struct TokenizerT_ {
	char *TString;
	struct TokenizerT_ *Sibling;
};

typedef struct TokenizerT_ TokenizerT;
//make a BST for delimiters to increase efficiancy
struct delimTree_ {
	char Ddelimiter;
	struct delimTree_ *leftChild;
	struct delimTree_ *rightChild;
};


typedef struct delimTree_ delimTree;


TokenizerT *TKCreate(char *separators, char *ts);
void createDelimTree(char *separators);
int checkCharToDelim(char c, delimTree *d);
delimTree *CreateD(char delim);
void addD(delimTree *toBeAdded, delimTree *tree);
void TKDestroy(TokenizerT *tk);
char *TKGetNextToken(TokenizerT *tk);
void DDestroy(delimTree *D);
/*
 * TKCreate creates a new TokenizerT object for a given set of separator
 * characters (given as a string) and a token stream (given as a string).
 * 
 * TKCreate should copy the two arguments so that it is not dependent on
 * them staying immutable after returning.  (In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */
delimTree *Root;
TokenizerT *TRoot;
//create 1 tokenizerT struct holding separators and string
TokenizerT *TKCreate(char *separators, char *ts) {
	int tslength = strlen(ts);
	char *ts2 = (char*)malloc(sizeof(char) * tslength);
	//char *ts2;
	TokenizerT *newTK = malloc(sizeof(TokenizerT));
	newTK->Sibling = NULL;
	//newTK->TString = malloc(sizeof(char) * tslength);
	strcpy(ts2, ts);
	//free(ts);
	newTK->TString = ts2;
	if(TRoot == NULL){
		TRoot = newTK;
	} 	 
	else{
		TokenizerT *traverse = TRoot;
		while(traverse->Sibling != NULL){
			traverse = traverse->Sibling;
		}
		traverse->Sibling = newTK;
	}
	//free(separators);
	return NULL;
}
//method to create the delimiter tree for quick and easy search
void createDelimTree(char *separators){
	int seplength = strlen(separators) +1;
	char *separators2;
	separators2 = (char*)malloc(seplength * sizeof(char));
	strcpy(separators2, separators);
	int j;
	for(j = 0; j < seplength; j++){
		delimTree *newNode = CreateD(separators2[j]);
		addD(newNode, Root);
	}
	free(separators2);
}
//check if character is delimiter return 1 for true 0 for false
int checkCharToDelim(char c, delimTree *d){

	char a = (*d).Ddelimiter;
	if(c == a){
		return 1;
	}
	if(c < a){
		if(d->leftChild == NULL){
			return 0;
		}
		return checkCharToDelim(c, d->leftChild);
	}
	if(c > a){
		if(d->rightChild == NULL){
			return 0;
		}
		return checkCharToDelim(c, d->rightChild);
	}
	return 0;	
}

//create a new delimTree to add
delimTree *CreateD(char delim){
	delimTree *toAdd = malloc(sizeof(delimTree));
	(*toAdd).Ddelimiter = delim;
	toAdd->leftChild = NULL;
	toAdd->rightChild = NULL;
	return toAdd;
}
// add delimTree *toBeAdded to tree
void addD(delimTree *toBeAdded, delimTree *tree){
	if(Root == NULL){
		Root = toBeAdded;
		return;
	}
	char addedD;
	char treeD;
	if(toBeAdded != NULL){
	addedD = (*toBeAdded).Ddelimiter;
	}
	if(tree != NULL){
	treeD = (*tree).Ddelimiter;
	}
	int strcomp = 2;
	//string comparison 1 if node to be added goes left, 0 if right
	if(addedD < treeD){
		strcomp = 1;
	}
	else if(addedD == treeD){
		free(toBeAdded);
		return;
	}
	else{
		strcomp = 0;
	}
	//go to leftchild
	if(strcomp == 1){
		if(tree->leftChild == NULL){
			tree->leftChild = toBeAdded;
			return;
		}
		else{
			addD(toBeAdded, tree->leftChild);
		}
	}
	//go to right child
	if(strcomp == 0){
		if(tree->rightChild == NULL){
			tree->rightChild = toBeAdded;
			return;
		}
		else{
			addD(toBeAdded, tree->rightChild);
		}
	} 	
}
/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */

void TKDestroy(TokenizerT *tk) {
	TokenizerT *tmp;
	tk = TRoot;
	while(tk != NULL){
		tmp = tk;
		tk = tk->Sibling;
		free(tmp->TString);
		free(tmp);
	}
}

/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

char *TKGetNextToken(TokenizerT *tk) {
	int stlength;
	if(tk->TString != NULL){
	stlength = strlen(tk->TString);
	}
	else{
	   stlength = 0;
	}
	char *nextline = (char*)malloc(sizeof(char) * stlength);
	if(strlen(tk->TString) == 1){
	nextline[0] = tk->TString[0];
	nextline[1] = '\0';
	}
	else{
	strcpy(nextline, tk->TString); 
	}
	return nextline;
}
/*Method to check for hex chars needed
 *\n = [0x0a]
 *\t = [0x09]
 *\v = [0x0b]
 *\b = [0x08]
 *\r = [0x0d]
 *\f = [0x0c]
 *\a = [0x07]
 *\\ = [0x5c]
 *\" = [0x22]
 * */
char *getHex(char *stringCheck){
	int numHexReplace = 0;
	int strCheckLength = strlen(stringCheck);
	int i;
	for(i = 0; i < strCheckLength; i++){
		if(stringCheck[i] == '\\'){
			if(i + 1 != strCheckLength){
				if(stringCheck[i+1] == 'n' || stringCheck[i+1] =='t' || stringCheck[i+1] == 'v' || stringCheck[i+1] == 'b' || stringCheck[i+1] == 'r' || stringCheck[i+1] == 'f' || stringCheck[i+1] == 'a' || stringCheck[i+1] == '\\' || stringCheck[i+1] =='"' ){
					numHexReplace++;
				}
			}
		}

	}
	if(numHexReplace == 0){
		return stringCheck;
	}
	else{
		int newSpaceNeeded1 = strCheckLength - (numHexReplace * 2);
		int newSpaceNeeded2 = numHexReplace * 6 + newSpaceNeeded1;
		char *newString = (char*)malloc(sizeof(char) * newSpaceNeeded2);
		int j = 0;
		int oph;
		for(oph = 0; oph < strCheckLength; oph++){
			if(stringCheck[oph] == '\\'){
				if(stringCheck[oph + 1] == 'n'){
					newString[j] = '[';
					newString[j+1] = '0';
					newString[j+2] = 'x';
					newString[j+3] = '0';
					newString[j+4] = 'a';
					newString[j+5] = ']';
					j = j + 5; 
					oph++;
				}
				else if(stringCheck[oph + 1] == 't'){
					newString[j] = '[';
					newString[j+1] = '0';
					newString[j+2] = 'x';
					newString[j+3] = '0';
					newString[j+4] = '9';
					newString[j+5] = ']';
					j = j + 5;
					oph++;
				}
				else if(stringCheck[oph + 1] == 'v'){
					newString[j] = '[';
					newString[j+1] = '0';
					newString[j+2] = 'x';
					newString[j+3] = '0';
					newString[j+4] = 'b';
					newString[j+5] = ']';
					j = j + 5;
					oph++;
				}
				else if(stringCheck[oph + 1] == 'b'){
					newString[j] = '[';
					newString[j+1] = '0';
					newString[j+2] = 'x';
					newString[j+3] = '0';
					newString[j+4] = '8';
					newString[j+5] = ']';
					j = j + 5;
					oph++;
				}
				else if(stringCheck[oph + 1] == 'r'){
					newString[j] = '[';
					newString[j+1] = '0';
					newString[j+2] = 'x';
					newString[j+3] = '0';
					newString[j+4] = 'd';
					newString[j+5] = ']';
					j = j + 5;
					oph++;
				}
				else if(stringCheck[oph + 1] == 'f'){
					newString[j] = '[';
					newString[j+1] = '0';
					newString[j+2] = 'x';
					newString[j+3] = '0';
					newString[j+4] = 'c';
					newString[j+5] = ']';
					j = j + 5;
					oph++;
				}
				else if(stringCheck[oph + 1] == 'a'){
					newString[j] = '[';
					newString[j+1] = '0';
					newString[j+2] = 'x';
					newString[j+3] = '0';
					newString[j+4] = '7';
					newString[j+5] = ']';
					j = j + 5;
					oph++;
				}

				else if(stringCheck[oph + 1] == '\\'){
					newString[j] = '[';
					newString[j+1] = '0';
					newString[j+2] = 'x';
					newString[j+3] = '5';
					newString[j+4] = 'c';
					newString[j+5] = ']';
					j = j + 5;
					oph++;
				}
				else if(stringCheck[oph + 1] == '"'){
					newString[j] = '[';
					newString[j+1] = '0';
					newString[j+2] = 'x';
					newString[j+3] = '2';
					newString[j+4] = '2';
					newString[j+5] = ']';
					j = j + 5;
					oph++;
				}

			}
			else{
				newString[j] = stringCheck[oph];
			}
			j++;
		} 
		free(stringCheck);
		return newString;
	}

}
/*
 * main will have two string arguments (in argv[1] and argv[2]).
 * The first string contains the separator characters.
 * The second string contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */

/* Now destroy the Delimiter Tree*/
void DDestroy(delimTree *D){
	if(D->leftChild != NULL && D->rightChild !=NULL){
		DDestroy(D->leftChild);
		DDestroy(D->rightChild);
		//free((*D).Ddelimiter);
		free(D);
		return;
	}
	else if(D->leftChild != NULL && D->rightChild == NULL){
		DDestroy(D->leftChild);
		free(D);
		return;
	}
	else if(D->rightChild != NULL && D->leftChild == NULL){
		DDestroy(D->rightChild);
		//free((*D).Ddelimiter);
		free(D);
		return;
	}
	else{
		free(D);
		return;
	}
}
/*char *freeAdd(int length){

}*/
int main(int argc, char **argv) {
	if(argc > 3){
		printf("Too many => error \n");
		return 0;
	}
	if(argc < 3){
		printf("Too few => error \n");
		return 0;
	}
	if(strcmp(argv[2], "") == 0){
	return 0;
	}
	createDelimTree(argv[1]);
	int i = 0;
	int tlength = strlen(argv[2]);
	int llength = strlen(argv[2]);
	char *add;
	add = (char*)malloc(tlength * sizeof(char));
	//strcpy(add, argv[2]);
	for(i = 0; i < tlength; i++){
		llength--;
			if(checkCharToDelim(argv[2][i],Root) == 1){
			if(strcmp(add, "") != 0){
				//TokenizerT *newTokeT = malloc(sizeof(TokenizerT));
				TokenizerT *newTokeT = TKCreate(add, add);
				//int l = strlen(add);
				//add[l + 1] = '\0';
				free(newTokeT);
				free(add);
				add = (char*) malloc(llength * sizeof(char));
			}
		}
		else{
			int j = strlen(add);
			add[j] = argv[2][i];
			/*if(i == strlen(argv[2]) - 1){
			  TokenizerT *newTokeT = malloc(sizeof(TokenizerT));
			  newTokeT = TKCreate(add, add);
			  }*/
		}
	}
	if(strcmp(add, "") != 0){
		//TokenizerT *newTokeT = malloc(sizeof(TokenizerT));
		TokenizerT *newTokeT = TKCreate(add, add);
		free(newTokeT);
		free(add);

	}

	TokenizerT* traverse1 = TRoot;
	while(traverse1->Sibling != NULL){
		char *nextline;
		nextline = TKGetNextToken(traverse1);
		nextline = getHex(nextline);
		printf("%s \n" , nextline);
		free(nextline);
		traverse1 = traverse1->Sibling;
	}
	char *nextline;
	nextline = TKGetNextToken(traverse1);
	nextline = getHex(nextline);
	printf("%s \n", nextline);
	free(nextline);		
	TKDestroy(TRoot);
	DDestroy(Root);
	return 0;
}
