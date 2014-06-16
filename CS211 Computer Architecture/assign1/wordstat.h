#ifndef WORDSTAT_H_INCLUDED
#define WORDSTAT_H_INCLUDED
#include<stdlib.h>
#include<stdio.h>
#include<malloc.h>
#include <string.h>
typedef struct node{
char *wordwcase;
char *wordwocase;
struct node *leftchild;
struct node *rightchild;
struct node *sibling;
int numOfOccur;
int numOfCaseOccur;	
}NODE;


void add(NODE *nodeToAdd, NODE *tree);
NODE* createNode(char *wordwcase, char *wordwocase);






#endif
