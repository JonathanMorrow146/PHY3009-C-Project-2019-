/*
 ============================================================================
 Name        : ProjectQ1_Y3.c
 Author      : Jonathan Morrow
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int main() {

	setvbuf(stdout,NULL,_IONBF,0);
	setvbuf(stderr,NULL,_IONBF,0);

	srand((long)time(NULL)); //required to make the random string more random - sets the seed value depending on the current time

	int i;
	double p;

	for(i=0; i<10; i++) //arbitrary iterating loop to print out 10 random number between 0 and 1
		{
			p = rand()/(double)RAND_MAX; //generates random numbers between 0 and 1
			printf("%g\n", p);
		} //loop will have generated 10 random numbers between 0 and 1
	return 0;
}
