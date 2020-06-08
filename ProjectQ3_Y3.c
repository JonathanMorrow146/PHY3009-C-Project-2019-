/*
 ============================================================================
 Name        : ProjectQ3_Y3.c
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
	int w; //arbitrary variable used in the array immunization
	int z; //arbitrary variable used in the spreading of the disease
	double p; //immunization probability
	int a, b, c; //counter variables for unimmunized, immunized and diseased, respectively

	p=0.4; //this is the immunization probability for the array, i.e. for 25*25 array, 250 points will be immunized
	a=0; //counter for unimmunized individuals
	b=0; //counter for immunized individuals
	c=0; //counter for diseased individuals

	for(j=0; j<=24; j++) //for loop to initialise array of randomly immunized/unimmunized individuals
	{
		for(i=0; i<=24; i++)
		{
			YX[j][i]=0;
		}
	}

	for(w=1; w<=(25*25)*p; w++) //gives the array an immunised probability of p=0.4
	{							//this loop will run until the array is immunized with the above probability
		r1 = (rand()/(double)RAND_MAX)*25; //produces random number between 0 and 24 inclusive
		r2 = (rand()/(double)RAND_MAX)*25;

		if(YX[r1][r2]==0) //checking if the randomised point is a zero, where 0 represents an unimmunized cell
		{
			YX[r1][r2]=1; //if a cell is unimmunized, set it equal to 1, where 1 represents an immunized cell
		}
		else
		{
			w--; //if the random y,x coincides with an already immunized cell, set the counter back one
		}
	}


	do{ //do loop to try and initiate disease by generating a random y,x until it lands on an unimmunized cell
		r1 = (rand()/(double)RAND_MAX)*25; //produces random number between 0 and 24 inclusive
		r2 = (rand()/(double)RAND_MAX)*25;

		if(YX[r1][r2]==0)
		{
			YX[r1][r2]=5; //representing an infected point with a 5
			printf("infection will start at %d, %d if %d, %d is unimmunized (array entry=0, note: array fills from top left)\n", r1, r2, r1, r2);
			printf("\n");
		}
		else
		{
			printf("Could not infect point %d,%d. Point was immunized.\n", r1, r2);
		}
	}while(YX[r1][r2]==1); //ensures that if the random y,x is immunized, it will re-run for a new random y,x until it corresponds with an unimmunized cell

	for(z=0; z<1000; z++)
	{
		for(j=0; j<=24; j++) //for loop to initialise array of randomly immunized/unimmunized individuals
		{
			for(i=0; i<=24; i++)
			{
				if(YX[j][i]==5)
				{
					if(YX[j-1][i]==0 && j!=0) //if the cell j-1,i is zero and j isn't zero, i.e, not at a corner, to prevent "wrapping" around to the far side of the array
					{
						YX[j-1][i]=5;
					}
					if(YX[j+1][i]==0 && j!=24) //if the cell j+1,i is zero and j isn't 24, i.e, not at a corner, to prevent "wrapping" around to the far side of the array
					{
						YX[j+1][i]=5;
					}
					if(YX[j][i-1]==0 && i!=0) //if the cell j,i-1 is zero and i isn't zero, i.e, not at a corner, to prevent "wrapping" around to the far side of the array
					{
						YX[j][i-1]=5;
					}
					if(YX[j][i+1]==0 && i!=24) //if the cell j,i+1 is zero and j isn't 24, i.e, not at a corner, to prevent "wrapping" around to the far side of the array
					{
						YX[j][i+1]=5;
					}
				}
				//printf("%d\t", YX[j][i]);
			}
			//printf("\n");
		}
		//printf("\n");
	}

	for(j=0; j<=24; j++)//nested for loops for counters a, b, c
	{
		for(i=0; i<=24; i++)
		{
			if(YX[j][i]==0)
			{
				a++; //counter for unimmunized individuals
			}
			if(YX[j][i]==1)
			{
				b++; //counter for immunized individuals
			}
			if(YX[j][i]==5)
			{
				c++; //counter for diseased individuals
			}
		}
	}//end of for loop to add to counters a,b,c for unimmunized, immunized, diseased

	printf("Unimmunized\tImmunized\tDiseased\n"); //print statements to return the number of Unimmunized, Immunized and Diseased individuals post simulation
	printf("%d\t%d\t%d\n", a, b, c);

	return 0;
}
