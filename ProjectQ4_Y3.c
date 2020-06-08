/*
 ============================================================================
 Name        : ProjectQ4_Y3.c
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

/*defining array with 'COLS' and 'ROWS' so that the code itself can be generalised,
allowing for considerable ease when wanting to change array dimensions */
#define COLS 25
#define ROWS 25
/*defining the number of simulations with 'NUM_SIM' to allow the user to input
however many simulations they wish to be performed for each immunization probability*/
#define NUM_SIM 50

//function declaration
int disease(int YX[COLS][ROWS], int r1, int r2);

int main() {

	setvbuf(stdout,NULL,_IONBF,0);
	setvbuf(stderr,NULL,_IONBF,0);

	srand((long)time(NULL)); //required to make the random string more random - sets the seed value depending on the current time

	//declaring variables
	int i, j; //column & row iterating variables
	int r1, r2; //variables assigned to random number generating processes
	int sim; //iterating variable to run for 'NUM_SIM' simulations
	int YX[COLS][ROWS]; //array with defined dimensions
	double p; //p is the fraction of immunized individuals
	double w; //arbitrary iterating variable used in the immuization of the array
	double a, b, c, a_tot, b_tot, c_tot; //counter variables for unimmunized, immunized and diseased; a,b,c are the per simulation values, a_tot, b_tot, c_tot are the values for 50 simulations
	double frac_DIS[NUM_SIM]; //array to store the fraction of diseased population out of the vulnerable population for each simulation
	double mf_DIS; //for a given immunization percentage, this is the mean fraction of diseased population out of the vulnerable population for 50 simulations
	double diffsq[NUM_SIM];//array to store the fractional difference squared of mf_DIS and frac_DIS[NUM_SIM] for each simulation of a given immunization probability/percentage
	double sum_diffsq;//variable to sum the elements of the array diffsq[NUM_SIM]
	double f_STDev;//variable to calculate the fractional standard deviation, over 'NUM_SIM' simulations, of a given immunization probability/percentage

	FILE * fp1;	//setting a file pointer to make and open a file with the name below
	fp1 = fopen("rawData.xls", "w"); //file containing data of numbers of unimmunized, immunized and diseased for 'NUM_SIM' simulations at the given immunization fraction
	FILE * fp2;
	fp2 = fopen("analysis.xls", "w"); //file containing immunization fraction, average fraction of diseased/vulnerable and fractional standard deviation, as well as the percentage counterparts

	p = 0.4 + 1449.0 / 50000.0; //probability to with which to set array immunization, given student number 40201449. General formula is p=0.4 + ((last 4 digits of student num)/50000.0)

	a_tot=0; //a_tot, b_tot, c_tot are counter variables for unimmunized, immunized and diseased individuals for 'NUM_SIM' simulations of a given immunization probability/percentage
	b_tot=0;
	c_tot=0;

	//setting the simulation to run NUM_SIM times
	for(sim=1; sim<=NUM_SIM; sim++)
	{
		a=0; //a,b,c are counter variables for unimmunized, immunized and diseased individuals, respectively, in each simulation. They must be reset at the beginning of each iteration.
		b=0;
		c=0;

		for(j=0; j<=(COLS-1); j++) //initially filling array with zeros, needs to be done inside the for loop for 'sim' so that a new, empty array is used in each iteration
		{
			for(i=0; i<=(ROWS-1); i++)
			{
				YX[j][i]=0;
			}
		}

		for(w=1; w<=(COLS*ROWS)*p; w++) //gives the array an immunized probability of p=0.4 + 1449/50000
		{						//this loop will run until the array is immunized with the above probability
			r1 = (rand()/(double)RAND_MAX)*COLS; //produces random number between 0 and the number assigned to 'COLS'-minus-1 inclusive
			r2 = (rand()/(double)RAND_MAX)*ROWS;

			if(YX[r1][r2]==0) //checking if the randomised point is a zero, where 0 represents an unimmunized cell
			{
				YX[r1][r2]=1; //if a cell is unimmunized, set it equal to 1, where 1 represents an immunized cell
			}
			else			//if the random y,x coincides with an already immunized cell, set the counter back one
			{
				w--;
			}
		}

		do{ //do loop to try and initiate disease by generating a random y,x until it lands on an unimmunized cell
			r1 = (rand()/(double)RAND_MAX)*COLS; //produces random number between 0 and 24 inclusive
			r2 = (rand()/(double)RAND_MAX)*ROWS;

			if(YX[r1][r2]==0)
			{
				YX[r1][r2]=5; //representing an infected point with a 5
			}
		}while(YX[r1][r2]==1); //ensures that if the random y,x is immunized, it will re-run for a new random y,x until it corresponds with an unimmunized cell

		disease(YX, r1, r2); //recursive function to spread the disease until every point in contact with a diseased point has been infected, unless it is an immunized point

		for(j=0; j<=(COLS-1); j++)//nested for loops for counters a, b, c
		{
			for(i=0; i<=(ROWS-1); i++)
			{
				if(YX[j][i]==0)
				{
					a++, a_tot++; //counter for unimmunized individuals
				}
				if(YX[j][i]==1)
				{
					b++, b_tot++; //counter for immunized individuals
				}
				if(YX[j][i]==5)
				{
					c++, c_tot++; //counter for diseased individuals
				}
			}
		}//end of for loop to add to counters a,b,c for unimmunized, immunized, diseased

		frac_DIS[sim] = (c / (a+c)); //array to store the fraction of diseased population (c) out of the vulnerable population (a+c) for each simulation

		if(sim==1) //setting headers for when data is placed in an external file, e.g. a .xls file
		{
			fprintf(fp1, "Unimmunized\tImmunized\tDiseased\tFrac Dis / Vuln\n");
			printf("frac imm\tmean frac caught\tfrac std\tpc imm\tmean pc caught\tpc std\n");
			fprintf(fp2, "frac imm\tmean frac caught\tfrac std\tpc imm\tmean pc caught\tpc std\n");
		}

		fprintf(fp1, "%g\t%g\t%g\t%g\n", a, b, c, frac_DIS[sim]); //printing the numbers of unimmunized, immunized, diseased and the fraction of diseased out of vulnerable individuals to the file associated with fp1

	}//end of 'sim' loop

	mf_DIS = (c_tot / (a_tot +c_tot)); //mean fraction diseased over 'NUM_SIM' simulations for the given immunization probability

	sum_diffsq = 0; //setting/reseting sum_diffsq to zero

	//for loop which calculates the fractional difference squared between frac_DIS[sim] and mf_DIS for the current 'sim' value over 'NUM_SIM' simulations and stores them within an array and then calculates their suman
	for(sim=1; sim<=NUM_SIM; sim++)
	{
		diffsq[sim] = (mf_DIS - frac_DIS[sim])*(mf_DIS - frac_DIS[sim]); //calculating the fractional difference squared between frac_DIS[sim] and mf_DIS for the current 'sim' value
		sum_diffsq = sum_diffsq + diffsq[sim]; //sum of array diffsq[sim] across the full range of 'sim' values
	}
	//calculating the fractional standard deviation from the square root of the sum of the difference squared between frac_DIS[sim] and mf_DIS in each simulation, divided by the number of simulations 'NUM_SIM'
	f_STDev = sqrt(sum_diffsq / NUM_SIM);

	//printing the fraction of immunization, mean fraction of diseased individuals, fractional standard deviation, and the percentage counterparts
	printf("%g\t%g\t%g\t%g\t%g\t%g\n", p, mf_DIS, f_STDev, (p*100), (mf_DIS*100), (f_STDev*100));
	fprintf(fp2, "%g\t%g\t%g\t%g\t%g\t%g\n", p, mf_DIS, f_STDev, (p*100), (mf_DIS*100), (f_STDev*100)); //printing the aforementioned quantities to the file named "analysis.xls"

	//closing each file to prevent any further writing
	fclose(fp1);
	fclose(fp2);

	return 0;
}

//function definition
//recursive function which when called will spread the disease until every point in contact with a diseased point has been infected, unless it is an immunized point
int disease(int YX[COLS][ROWS], int r1, int r2)
{
		if(YX[r1-1][r2]==0 && r1!=0) //if the cell j-1,i is zero and j isn't zero, i.e, not at a corner, to prevent "wrapping" around to the far side of the array
		{
			YX[r1-1][r2]=5;
			disease(YX, (r1-1), r2);
		}
		if(YX[r1+1][r2]==0 && r1!=(COLS-1))//if the cell j+1,i is zero and j isn't 24, i.e, not at a corner, to prevent "wrapping" around to the far side of the array
		{
			YX[r1+1][r2]=5;
			disease(YX, (r1+1), r2);
		}
		if(YX[r1][r2-1]==0 && r2!=0)//if the cell j,i-1 is zero and i isn't zero, i.e, not at a corner, to prevent "wrapping" around to the far side of the array
		{
			YX[r1][r2-1]=5;
			disease(YX, r1, (r2-1));
		}
		if(YX[r1][r2+1]==0 && r2!=(ROWS-1))//if the cell j,i+1 is zero and i isn't 24, i.e, not at a corner, to prevent "wrapping" around to the far side of the array
		{
			YX[r1][r2+1]=5;
			disease(YX, r1, (r2+1));
		}
		return 0;
}
