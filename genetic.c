/*------------------------------------------------------------------
 *	This code implements an iterative Genetic Algorithm
 *	to show improved fitness over time.
 * ----------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define ORG_LENGTH 2
#define BUF_SIZE 17

typedef struct {

	unsigned int gene[2];
	unsigned int fitness;

}organism;

char *int2bin(int a, char *buffer, int buf_size);
int Fitness(unsigned int gene[]);


int main (int argc, char* argv[])
{
	int i,j,k;
	//unsigned int seed = 923859;
	srand48(time(NULL));
	int POPSIZE = atoi(argv[1]);
	int NUM_GENERATIONS = atoi(argv[2]);
	float CROSSOVER = atof(argv[3]);
	float MUTATION = atof(argv[4]);

/*---------------------Randomly initialize Population----------------*/

	organism* population = malloc(POPSIZE*sizeof(organism));;
	organism* parent = malloc(2*sizeof(organism));
	
	for (i=0; i<POPSIZE; i++)
	{
		population[i].fitness = 0;
		for (j=0; j<2; j++)
		{
			population[i].gene[j] = drand48()*(ORG_LENGTH*sizeof(int)*8);
		}
	}
	int TOTAL_FITNESS = 0;

	for (k=0; k<NUM_GENERATIONS; k++)
	{
/*------------------------------Selection-----------------------------*/
		int count,r;
		for (count=0; count<2; count++)
		{
			int sumFitness = 0;
			for (r=0; r<POPSIZE; r++)
			{
				sumFitness += population[i].fitness;
				int RANDOM = lrand48() % sumFitness;	
				if (sumFitness >= RANDOM)
				{
					parent[count] = population[i];
				}
			}
		}

/*-------------------------------Crossover---------------------------*/

		unsigned int mask[2][ORG_LENGTH];
		unsigned int temp[4][ORG_LENGTH];
		int crosspt,first,second,i,j;
		float RANDOM2 = drand48();

		if (RANDOM2 < CROSSOVER)
		{	
			crosspt = lrand48()%(ORG_LENGTH*sizeof(int)*8);
			first = crosspt/(sizeof(int)*8);
			second = crosspt%(sizeof(int)*8);
			for (i=0; i<ORG_LENGTH; i++)
			{
				if (i<first)
				{
					mask[0][i] = 0x0000;
					mask[1][i] = 0xFFFF;
				}
				else
				{
					mask[0][i] = 0xFFFF;
					mask[1][i] = 0x0000;
				}
			}
			mask[1][first] = mask[0][first]<<((sizeof(int)*8)-second);
			mask[0][first] = mask[0][first]>>second;
			for (j=0; j<ORG_LENGTH; j++)
			{
				temp[0][j] = parent[0].gene[j] & mask[0][j];
				temp[1][j] = parent[0].gene[j] & mask[1][j];
				temp[2][j] = parent[1].gene[j] & mask[0][j];
				temp[3][j] = parent[1].gene[j] & mask[1][j];
				
				parent[0].gene[j] = temp[0][j] | temp[3][j];
				parent[1].gene[j] = temp[2][j] | temp[4][j];	
			}
		}
				
/*------------------------Mutate Chromosome---------------------------*/

		RANDOM2 = drand48();
		if(RANDOM2 < MUTATION)
		{
			for (count=0; count<2; count++)
			{	
				int mutation = lrand48()%(ORG_LENGTH*sizeof(int)*8);
				int mask = 1 << (mutation%(sizeof(int)*8)-1);
				parent[count].gene[(int)mutation/(sizeof(int)*8)] ^= mask;
			}
		}

		parent[0].fitness += Fitness(parent[0].gene);
		parent[1].fitness += Fitness(parent[1].gene);
		TOTAL_FITNESS += parent[0].fitness + parent[1].fitness;

		int RANDOM3 = lrand48()%POPSIZE;
		for (count =0; count<2; count++)
		{
			population[RANDOM3] = parent[count];
		}

	}
	
/*--------------------------Print Chromosomes---------------------------*/
	char buffer[BUF_SIZE];
	buffer[BUF_SIZE - 1] = '\0';
	
	for (i=0; i<POPSIZE; i++)
	{
		for(j=0;j<ORG_LENGTH; j++)
		{
			int2bin(population[i].gene[j], buffer, BUF_SIZE - 1);
			printf("%s", buffer);
		}
		printf("\n");
		printf("Fitness: %d\n", population[i].fitness);
	}
	printf("TOTAL FITNESS AFTER %d GENERATIONS: %d\n", NUM_GENERATIONS, TOTAL_FITNESS);
	
	free(parent);	
	free(population);

	return 0;
}

/*---------------------------Fitness----------------------------------*/
int Fitness(unsigned int gene[])
{
	unsigned int i=0;
	int j;
	for (j=0; j<ORG_LENGTH; j++)
	{
		unsigned int temp = gene[j];
		for (; temp; i++)
		{
			temp &= temp - 1;
		}
	}
	return i;
}

/*-------------------Convert int to Binary------------------------*/
	
char *int2bin(int a, char *buffer, int buf_size)
{
	int i;
	buffer += (buf_size - 1);
	for (i=31; i>=0; i--)
	{
		*buffer-- = (a & 1) + '0';
		a >>= 1;
	}
	return buffer;
}
