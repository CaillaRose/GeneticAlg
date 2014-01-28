#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define POPSIZE 100
#define NUM_GENERATIONS 400
#define ORG_LENGTH 2
#define MUTATION 0.001
#define CROSSOVER 0.7

typedef struct {

	unsigned int gene[ORG_LENGTH];
	unsigned int fitness;

}organism;

int main ()
{
	int i,j,k;
	double RANDOM = 0.0f;
	unsigned int seed = 923859;
	srand48(seed);

/*------------------Randomly initialize Population----------------*/

	organism population[POPSIZE];
	organism child[2];

	for (i=0; i<POPSIZE; i++)
	{
		population[i].gene = drand48()*(ORG_LENGTH*sizeof(int)*8);
		population[i].fitness = 0.0f;
	}
		
	int generation = 0;
	int MAXFITNESS_FLAG = 0;	//check if solution has been found

	while(MAXFITNESS_FLAG == 0)
	{
		//TEST FITNESS
		//CHECK FOR SOLUTION FOUND

		for (i=0; i<POPSIZE; i++)
		{
			if (population[i].fitness == 999.0f)
			{
				printf("Population has reached Maximum Fitness in %d Generations.", generation);
				MAXFITNESS_FLAG = 1;
				break;
			}
		}

		organism temp[POPSIZE];
		int matingpop = 0;
		while (matingpop < POPSIZE)
		{
			child[0] = Selection(fitness, population);
			child[1] = Selection(fitness, population);
			crossover(child);
			mutate(child[0]);
			mutate(child[1]);
			temp[matingpop++] = organism(child[0], 0.0f);
			temp[matingpop++] = organism(child[1], 0.0f);
		}

		for (i=0; i<POPSIZE; i++)
		{
			population[i] = temp[i];
		}

		++generation;
	}
}

/*----------------------Selection-----------------------------*/

void Selection(int fitness, organism* population)
{
	float sumFitness = 0.0f;
	int i;
	float RANDOM = (float)((drand48()%1000)/999.0f)*sumFitness;
	
	for (i=0; i<POPSIZE; i++)
	{
		sumFitness += population[i].fitness;
	
		if (sumFitness >= RANDOM)
		{
			return population[i].gene;
		}
	}
}		 

/*-------------------Crossover---------------------------*/

void crossover(organism *child)
{
	unsigned int mask[2][ORG_LENGTH];
	unsigned int temp[4][ORG_LENGTH];
	int crosspt,first,second,i,j;
	float RANDOM = drand48();

	if (RANDOM < CROSSOVER)
	{	
		crosspt = drand48()*(ORG_LENGTH*sizeof(int)*8);
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
			temp[0][j] = child[0].gene[j] & mask[0][j];
			temp[1][j] = child[0].gene[j] & mask[1][j];
			temp[2][j] = child[1].gene[j] & mask[0][j];
			temp[3][j] = child[1].gene[j] & mask[1][j];
			
			child[0].gene[j] = temp[0][j] | temp[3][j];
			child[1].gene[j] = temp[2][j] | temp[4][j];	
		}
}
/*--------------------Mutate Chromosome---------------------------*/

void mutate(organism *child)
{

	RANDOM = drand48();
	if(RANDOM < MUTATION)
	{
		int mutation = drand48()%(ORG_LENGTH*sizeof(int)*8);
		int mask = 1 << (mutation%(sizeof(int)*8)-1);
		child.gene[(int)mutation/(sizeof(int)*8)] ^= mask;
	}
	return;	
}

/*------------------Print Bit Array---------------------------*/

void print(int n)
{
	unsigned int mask = 1 << ((8*sizeof(int))-1);
	printf("\t%d\t=", n);
	while (mask > 0)
	{
		printf("%d", (n&mask)?0:1);
		mask >> 1;
	}
	printf("\n");
}
