#include <stdio.h>
#include "analyzecache.h"
#include <time.h>
#include <malloc.h>
#include <stdio.h>

#define KB 1024
#define MB 1024 * KB

int main(int argc, char *argv[])
{	
	clock_t start_t, end_t, step1_t, step2_t;
	int big = 8 * MB;
	char *BigArray = (char*)malloc(big);
	//memset(BigArray, 0, big);
	int f = 0;
	while(f < big){
	     if(f % 2 == 0){
		BigArray[f] = 'a';
	     }
	     else{
		BigArray[f] = 'b';
	     }
	     f++;
	}
	int n = 0;
	int j = 1;
		while(j + n < big){
			step1_t = clock();
			step2_t = clock();
			j++;
			n++;
		}	
	
	return 0;
}
