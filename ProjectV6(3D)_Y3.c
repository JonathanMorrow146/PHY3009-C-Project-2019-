/*
 ============================================================================
 Name        : ProjectV6(3D)_Y3.c
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

/*defining array with 'DIM1', 'DIM2', 'DIM3' so that the code itself can be generalised,
allowing for considerable ease when wanting to change array dimensions */
#define DIM1 9
#define DIM2 9
#define DIM3 9
/*defining the number of simulations with 'NUM_SIM' to allow the user to input
however many simulations they wish to be performed for each immunization probability*/
#define NUM_SIM 50

//function declaration
int disease(int YXZ[DIM1][DIM2][DIM3], int r1, int r2, int r3);

int main() {

	setvbuf(stdout,NULL,_IONBF,0);
	setvbuf(stderr,NULL,_IONBF,0);

	srand((long)time(NULL)); //required to make the random string more random - sets the seed value depending on the current time

	//declaring variables
	int i, j, k; //DIM1, DIM2 and DIM3 iterating variables
	int r1, r2, r3; //variables assigned to random number generating processes
	int sim; //iterating variable to run for 'NUM_SIM' simulations
	int YXZ[DIM1][DIM2][DIM3]; //array with defined dimensions
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
	fp2 = fopen("analysis.xls", "w"); //file containing percentage immunisation, average fraction of diseased and standard deviation, average percentage of diseased and standard deviation

	p = 0.4 + 1449.0 / 50000.0; //probability to with which to set array immunization, given student number 40201449

	a_tot=0;
	b_tot=0;
	c_tot=0;

	//setting the simulation to run 50 times
	for(sim=1; sim<=NUM_SIM; sim++)
	{
		a=0; //a,b,c are counter variables for unimmunized, immunized and diseased individuals, respectively, in each simulation. They must be reset at the beginning of each iteration.
		b=0;
		c=0;

		for(j=0; j<=(DIM1-1); j++) //initially filling array with zeros, needs to be done inside the for loop for 'sim' so that a new, empty array is used in each iteration
		{
			for(i=0; i<=(DIM2-1); i++)
			{
				for(k=0; k<=(DIM3-1); k++)
				{
					YXZ[j][i][k]=0;
				}
			}
		}

		for(w=1; w<=(DIM1*DIM2*DIM3)*p; w++) //gives the array an immunised probability of p=0.4 + 1449/50000
		{						//this loop will run until the array is immunised with the above probability
			r1 = (rand()/(double)RAND_MAX)*DIM1; //produces random number between 0 and (DIM1 - 1) inclusive
			r2 = (rand()/(double)RAND_MAX)*DIM2;
			r3 = (rand()/(double)RAND_MAX)*DIM3;

			if(YXZ[r1][r2][r3]==0) //checking if the randomised point is a zero, where 0 represents an unimmunized cell
			{
				YXZ[r1][r2][r3]=1; //if a cell is unimmunized, set it equal to 1., where 1 represents an immunized cell
			}
			else			//if the random y,x coincides with an already immunized cell, set the counter back one
			{
				w--;
			}
		}

		do{ //do loop to try and initiate disease by generating a random y,x,z until it lands on an unimmunized cell
			r1 = (rand()/(double)RAND_MAX)*DIM1; //produces random number between 0 and (DIM1 - 1) inclusive
			r2 = (rand()/(double)RAND_MAX)*DIM2;
			r3 = (rand()/(double)RAND_MAX)*DIM3;

			if(YXZ[r1][r2][r3]==0)
			{
				YXZ[r1][r2][r3]=5; //representing an infected point with a 5
			}
		}while(YXZ[r1][r2][r3]==1); //ensures that if the random y,x,z is immunised, it will re-run for a new random y,x,z until it corresponds with an unimmunized cell

		disease(YXZ, r1, r2, r3); //recursive function to spread the disease until every point in contact with a diseased point has been infected, unless it is an immunized point

		for(j=0; j<=(DIM1-1); j++)//nested for loops for counters a, b, c for the current 'sim' iteration
		{
			for(i=0; i<=(DIM2-1); i++)
			{
				for(k=0; k<=(DIM3-1); k++)
				{
					if(YXZ[j][i][k]==0)
					{
						a++, a_tot++; //counter for unimmunized individuals
					}
					if(YXZ[j][i][k]==1)
					{
						b++, b_tot++; //counter for immunized individuals
					}
					if(YXZ[j][i][k]==5)
					{
						c++, c_tot++; //counter for diseased individuals
					}
				}
			}
		}//end of for loop to add to counters a,b,c for unimmunized, immunized, diseased

		frac_DIS[sim] = (c / (a+c)); //array to store the fraction of diseased population (c) out of the vulnerable population (a+c) for each simulation

		//the following nested 'if' statements set column headers at the beginning of each excel file
			if(sim==1)
			{
				fprintf(fp1, "Unimmunized\tImmunized\tDiseased\tFrac Dis / Vuln\n");
				printf("frac imm\tmean frac caught\tfrac std\tpc imm\tmean pc caught\tpc std\n");
				fprintf(fp2, "frac imm\tmean frac caught\tfrac std\tpc imm\tmean pc caught\tpc std\n");
			}
			fprintf(fp1, "%g\t%g\t%g\t%g\n", a, b, c, frac_DIS[sim]); //printing the numbers of unimmunized, immunized, diseased and the fraction of diseased out of vulnerable individuals to the file associated with fp1

	}//end of 'sim' loop

	mf_DIS = (c_tot / (a_tot +c_tot)); //mean fraction diseased over 'NUM_SIM' simulations for the given immunization probability

	sum_diffsq = 0; //setting/reseting sum_f_STD to zero

	//for loop which calculates the fractional difference squared between frac_DIS[sim] and mf_DIS for the current 'sim' value over 'NUM_SIM' simulations and stores them within an array and then calculates their sum
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
//end of main

//function definition
//recursive function which when called will spread the disease until every point in contact with a diseased point has been infected, unless it is an immunized point
int disease(int YXZ[DIM1][DIM2][DIM3], int r1, int r2, int r3)
{
		if(YXZ[r1-1][r2][r3]==0 && r1!=0) //if the cell r1-1,r2,r3 is zero and r1 isn't zero, i.e, not at a corner, to prevent "wrapping" around to the far side of the array
		{
			YXZ[r1-1][r2][r3]=5;
			disease(YXZ, (r1-1), r2, r3); //recalling (hence recursion) the entire function from the beginning, in this case checking the statement again if it was initially true
		}
		if(YXZ[r1+1][r2][r3]==0 && r1!=(DIM1-1))//if the cell r1+1,r2,r3 is zero and r1 isn't 'DIM1-1', i.e, not at a corner, to prevent "wrapping" around to the far side of the array
		{
			YXZ[r1+1][r2][r3]=5;
			disease(YXZ, (r1+1), r2, r3);
		}
		if(YXZ[r1][r2-1][r3]==0 && r2!=0)//if the cell r1,r2-1,r3 is zero and r2 isn't zero, i.e, not at a corner, to prevent "wrapping" around to the far side of the array
		{
			YXZ[r1][r2-1][r3]=5;
			disease(YXZ, r1, (r2-1), r3);
		}
		if(YXZ[r1][r2+1][r3]==0 && r2!=(DIM2-1))//if the cell r1,r2+1,r3 is zero and r2 isn't 'DIM2-1', i.e, not at a corner, to prevent "wrapping" around to the far side of the array
		{
			YXZ[r1][r2+1][r3]=5;
			disease(YXZ, r1, (r2+1), r3);
		}
		if(YXZ[r1][r2][r3-1]==0 && r3!=0)//if the cell r1,r2,r3-1 is zero and r3 isn't zero, i.e, not at a corner, to prevent "wrapping" around to the far side of the array
			{
				YXZ[r1][r2][r3-1]=5;
				disease(YXZ, r1, r2, (r3-1));
			}
		if(YXZ[r1][r2][r3+1]==0 && r3!=(DIM3-1))//if the cell r1,r2,r3+1 is zero and r3 isn't 'DIM3-1', i.e, not at a corner, to prevent "wrapping" around to the far side of the array
		{
			YXZ[r1][r2][r3+1]=5;
			disease(YXZ, r1, r2, (r3+1));
		}
		return 0;
}
