#include "analyzecache.h"
#include <time.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>


#define KB 1024
#define MB 1024 * KB

int main(int argc, char *argv[])
{	
	clock_t start_t, end_t, step1_t, step2_t;
	int big = 8 * MB;
	char *BigArray = (char*)malloc(big);
	memset(BigArray, 0, big);
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

	double power = 1;
	static double checknum = 0;
	//traversal to get mapped to physical memory
	for(power = 1; checknum < big; power++){
		checknum = pow(2, power);
	}
	
	checknum = 0;
	for(power = 1; checknum < big; power++){
		checknum = pow(2, power);
		step1_t = clock();
                char a = BigArray[(int)checknum];
                char b = BigArray[(int)checknum/2];
                char c = a + b;
                step2_t = clock();
                end_t = (double)(step1_t - step2_t) / CLOCKS_PER_SEC;
                printf("Time for loop was: %f\n", end_t);
	}

	return 0;
}
