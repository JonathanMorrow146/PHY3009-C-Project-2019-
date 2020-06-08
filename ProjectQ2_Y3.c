/*
 ============================================================================
 Name        : ProjectQ2_Y3.c
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

	int YX[25][25]; //two dimensional, 25 by 25 array
	int i, j; //iterating variables for array
	int r1, r2; //variables for the random numbers
	int w; //abitrary variable used in the array immunization
	double p; //immunization probability

	p=0.4; //this is the immunization probability for the array, i.e. for 25*25 array, 250 points will be immunized

	for(j=0; j<=24; j++) //nested for loops to initially populate array with zeros
	{
		for(i=0; i<=24; i++)
		{
			YX[j][i]=0;
		}
	}

	for(w=1; w<=(25*25)*p; w++) //gives the array an immunized probability of p=0.4
	{							//this loop will run until the array is immunized with the above probability
		r1 = (rand()/(double)RAND_MAX)*24; //produces random number between 0 and 24 inclusive
		r2 = (rand()/(double)RAND_MAX)*24;

			if(YX[r1][r2]==0) //checking if the randomised point is a zero, where 0 represents an unimmunized cell
			{
				YX[r1][r2]=1; //if a cell is unimmunized, set it equal to 1, where 1 represents an immunized cell
			}
			else			//if the random y,x coincides with an already immunized cell, set the counter back one
			{
				w--;
			}
	}

	for(j=0; j<=24; j++) //printing array to see if it has been properly populated with zeros and ones with the correct immunization probability
	{
		for(i=0; i<=24; i++)
		{
			printf("%d\t", YX[i][j]);
		}
		printf("\n");
	}

	return 0;
}
