/*
 ============================================================================
 Name        : Project(RP3D)_Y3.c
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
int infect(int YXZ[DIM1][DIM2][DIM3], int r1, int r2, int r3);
int disease(int YXZ[DIM1][DIM2][DIM3], int r1, int r2, int r3);

int main() {

	setvbuf(stdout,NULL,_IONBF,0);
	setvbuf(stderr,NULL,_IONBF,0);

	srand((long)time(NULL)); //required to make the random string more random - sets the seed value depending on the current time

	//declaring variables
	int i, j, k; //column & row iterating variables
	int r1, r2, r3; //variables assigned to random number generating processes
	int sim; //iterating variable to run for 'NUM_SIM' simulations (in this case NUM_SIM=50)
	int m; //iterating variable for percentage immunisation from 10% to 90%
	int YXZ[DIM1][DIM2][DIM3]; //array with defined dimensions
	double p; //p is the fraction of immunized individuals, (=m/100)
	double w; //arbitrary iterating variable used in the immuization of the array
	double a, b, c, a_tot, b_tot, c_tot; //counter variables for unimmunized, immunized and diseased; a,b,c are the per simulation values, a2,b2,c2 are the values for 50 simulations
	double frac_DIS[NUM_SIM]; //array to store the fraction of diseased population out of the VULNERABLE population for each simulation
	double mf_DIS; //for a given immunization percentage, this is the mean fraction of diseased population out of the VULNERABLE population for 50 simulations
	double diffsq[NUM_SIM];//array to store the fractional difference squared of mf_DIS and frac_DIS[NUM_SIM] for each simulation of a given immunization probability/percentage
	double sum_diffsq;//variable to sum the elements of the array diffsq[NUM_SIM]
	double f_STDev;//variable to calculate the fractional standard deviation, over 'NUM_SIM' simulations, of a given immunization probability/percentage
	double frac_DIS_tot[NUM_SIM]; //array to store the fraction of diseased population out of the TOTAL population for each simulation
	double mf_DIS_tot;//for a given immunization percentage, this is the mean fraction of diseased population out of the TOTAL population for 50 simulations
	double diffsq_tot[NUM_SIM];//array to store the fractional difference squared of mf_DIS_tot and frac_DIS_tot[NUM_SIM] for each simulation of a given immunization probability/percentage
	double sum_diffsq_tot;//variable to sum the elements of the array diffsq_tot[NUM_SIM]
	double f_STDev_tot;//variable to calculate the fractional standard deviation, over 'NUM_SIM' simulations, of a given immunization probability/percentage

	FILE * fp1;	//setting a file pointer to make and open a file with the name below
	fp1 = fopen("rawData.xls", "w"); //file containing data of percentage immunised, numbers of unimmunized, immunized and diseased for 'NUM_SIM' simulations at each immunization percentage
	FILE * fp2;
	fp2 = fopen("analysis.xls", "w"); //file containing the fraction of immunization, mean fraction of diseased out of VULNERABLE individuals, fractional standard deviation for diseased/VULNERABLE, mean fraction of diseased out of TOTAL individuals, fractional standard deviation for diseased/TOTAL, and the percentage counterparts

	for(m=1; m<=100; m++) //m represents % of population immunized. This loops varies immunization percentage from 1-100%
	{
		p=(m/100.0); //changing percentage immunization to decimal
		a_tot=0; //a_tot, b_tot, c_tot are counter variables for unimmunized, immunized and diseased individuals for 'NUM_SIM' simulations of a given immunization probability/percentage
		b_tot=0;
		c_tot=0;


		//setting the simulation to run 'NUM_SIM' times
		for(sim=1; sim<=NUM_SIM; sim++)
		{
			a=0; //a,b,c are counter variables for unimmunized, immunized and diseased individuals, respectively, for each simulation. They must be reset at the beginning of each iteration.
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

			for(w=1; w<=(DIM1*DIM2*DIM3)*p; w++) //gives the array an immunised probability of p=m/100, where m is percentage immunization
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

			if(m==100) //condition for 100% immunization to prevent the program from getting stuck. In the event of full immunization, the program will bypass the 'infect' and 'disease' functions
			{
				break;
			}
			else
			{
			infect(YXZ, r1, r2, r3); //recursive function to try initiate the disease on an unimmunized point, which will rerun until the point generated via the function is indeed unimmunized
			disease(YXZ, r1, r2, r3); //recursive function to spread the disease until every point in contact with a diseased point has been infected, unless it is an immunized point
			}

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

			frac_DIS[sim] = (c / (a+c)); //array to store the fraction of diseased population (c) out of the VULNERABLE population (a+c) for each simulation
			frac_DIS_tot[sim] = ( c / (a+b+c)); //array to store the fraction of diseased population (c) out of the TOTAL population (a+c) for each simulation

			//the following nested 'if' statements set column headers at the beginning of each excel file
			if(m==1)
			{
				if(sim==1)
				{
					fprintf(fp1, "percentage Immunized\tUnimmunized\tImmunized\tDiseased\tFrac Dis/Vuln\tFrac Dis/Total\n");
					printf("frac immun\tav frac dis/vul\tfrac std dis/vuln\tav frac dis/tot\tfrac stdev dis/tot\tpc Imm\tav pc dis/vul\tpc std dis/vuln\tav pc dis/tot\tpc stdev dis/tot\n");
					fprintf(fp2, "frac immun\tav frac dis/vul\tfrac std dis/vuln\tav frac dis/tot\tfrac stdev dis/tot\tpc Imm\tav pc dis/vul\tpc std dis/vuln\tav pc dis/tot\tpc stdev dis/tot\n");
				}
			}

			fprintf(fp1, "%d\t%g\t%g\t%g\t%g\t%g\n", m, a, b, c, frac_DIS[sim], frac_DIS_tot[sim]); //printing to the "rawData.xls" file

		}//end of 'sim' loop

	mf_DIS = (c_tot / (a_tot +c_tot)); //mean fraction diseased/VULNERABLE over 'NUM_SIM' simulations for the given immunization probability
	mf_DIS_tot = (c_tot / (a_tot + b_tot + c_tot)); //mean fraction diseased/TOTAL over 'NUM_SIM' simulations for the given immunization probability

	sum_diffsq = 0; //setting/reseting sum_diffsq and sum_diffsq_tot to zero
	sum_diffsq_tot = 0;

	//for loop which calculates the fractional difference squared between frac_DIS[sim] with mf_DIS and frac_DIS_tot[sim] with mf_DIS_tot for the current 'sim' value over 'NUM_SIM' simulations and stores them within an array and then calculates their sum
	for(sim=1; sim<=NUM_SIM; sim++)
	{
		diffsq[sim] = (mf_DIS - frac_DIS[sim])*(mf_DIS - frac_DIS[sim]); //calculating the fractional difference squared between frac_DIS[sim] and mf_DIS for the current 'sim' value
		sum_diffsq = sum_diffsq + diffsq[sim]; //sum of array diffsq[sim] across the full range of 'sim' values

		diffsq_tot[sim] = (mf_DIS_tot - frac_DIS_tot[sim])*(mf_DIS_tot - frac_DIS_tot[sim]); //calculating the fractional difference squared between frac_DIS_tot[sim] and mf_DIS_tot for the current 'sim' value
		sum_diffsq_tot = sum_diffsq_tot + diffsq_tot[sim]; //sum of array diffsq_tot[sim] across the full range of 'sim' values
	}
	//calculating the average fractional standard deviation for the diseased/VULNERABLE and diseased/TOTAL from the sum of the fractional standard deviations in each simulation, divided by the number of simulations 'NUM_SIM'
	f_STDev = sqrt(sum_diffsq / NUM_SIM);
	f_STDev_tot = sqrt(sum_diffsq_tot / NUM_SIM);

	if(m==100) //conditions for 100% immunization so that data in an external file will return 0 instead of 'not a number' style errors
	{
		mf_DIS=0;
		f_STDev=0;
		mf_DIS_tot=0;
		f_STDev_tot=0;

	}

	//printing the fraction of immunization, mean fraction of diseased out of VULNERABLE individuals, fractional standard deviation for diseased/VULNERABLE, mean fraction of diseased out of TOTAL individuals, fractional standard deviation for diseased/TOTAL, and the percentage counterparts
	printf("%g\t%g\t%g\t%g\t%g\t%g\t%g\t%g\t%g\t%g\n", p, mf_DIS, f_STDev, mf_DIS_tot, f_STDev_tot, (p*100), (mf_DIS*100), (f_STDev*100), mf_DIS_tot*100, f_STDev_tot*100);
	fprintf(fp2, "%g\t%g\t%g\t%g\t%g\t%g\t%g\t%g\t%g\t%g\n", p, mf_DIS, f_STDev, mf_DIS_tot, f_STDev_tot, (p*100), (mf_DIS*100), (f_STDev*100), mf_DIS_tot*100, f_STDev_tot*100); //printing the aforementioned quantities to the file named "analysis.xls"
	}//end of immunization probability varying loop

	//closing each file to prevent any further writing
	fclose(fp1);
	fclose(fp2);

	return 0;
}
//end of main

//function definition
//recursive function to try initiate the disease on an unimmunized point, which will rerun until the point generated via the function is indeed unimmunized
int infect(int YXZ[DIM1][DIM2][DIM3], int r1, int r2, int r3)
{
	r1 = (rand()/(double)RAND_MAX)*DIM1;
	r2 = (rand()/(double)RAND_MAX)*DIM2;
	r3 = (rand()/(double)RAND_MAX)*DIM3;

	if(YXZ[r1][r2][r3]==0)
		{
			YXZ[r1][r2][r3]=5; //representing an infected point with a 5
		}
		else
		{
			infect(YXZ, r1, r2, r3);
		}
	return 0;
}

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
