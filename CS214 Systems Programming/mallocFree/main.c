#include "mymalloc.h"
#include <stdlib.h>
#include <stdio.h>

#define malloc(x) my_malloc(x)
#define free(x) my_free(x)
		 


int main(int argc, char **argv){
	char *large_frag = malloc(4862);
	char *small_frag = malloc(189);
	free(large_frag);
	free(small_frag);
	//saturation check
	char *satcheck = malloc(5000);
	//double free check
	char *doublecheck = malloc(200);
	free(doublecheck);
	free(doublecheck);
	
	//free pointer that wasn't allocated
	int x = 3;
	free(&x);
	
	//free pointer not allocated with malloc
	char *test1 = malloc(30);
	free(test1 + 10);
	exit(0);

}
