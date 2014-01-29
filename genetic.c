#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define POPSIZE 100
#define NUM_GENERATIONS 400
#define MUTATION 0.001
#define CROSSOVER 0.7

typedef struct {

	unsigned int gene[2];
	unsigned int fitness;

}organism;

int main ()
{
	int i,j,k;
	unsigned int seed = 923859;
	srand48(seed);

/*------------------Randomly initialize Population----------------*/

	organism **population = malloc(POPSIZE*sizeof(organism *));;
	
	for (i=0; i<POPSIZE; i++)
	{
		population[i] = malloc(sizeof(organism));
		population[i]->gene = drand48()*(ORG_LENGTH*sizeof(int)*8);
		population[i]->fitness = 0;
	}

	organism* child[2];
	
	for (k=0; k<GENERATION; k++)
	{
		for (i=0; i<POPSIZE; i++)
		{
			population[i]->gene = drand48()*(ORG_LENGTH*sizeof(int)*8);
			population[i]->fitness = 0;
		}

/*----------------------Selection-----------------------------*/

		for (count=0; count<2; count++)
		{
			int sumFitness = 0;
			for (r=0; r<POPSIZE; r++)
			{
				sumFitness += population[i]->fitness;
				float RANDOM = (float)((drand48()%1000)/999.0f)*sumFitness;	
				if (sumFitness >= RANDOM)
				{
					parent[count] = population[i];
				}
			}
		}

/*-------------------Crossover---------------------------*/

				
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
				
/*--------------------Mutate Chromosome---------------------------*/

		RANDOM = drand48();
		if(RANDOM < MUTATION)
		{
			int mutation = drand48()%(ORG_LENGTH*sizeof(int)*8);
			int mask = 1 << (mutation%(sizeof(int)*8)-1);
			child.gene[(int)mutation/(sizeof(int)*8)] ^= mask;
		}


	for (i=0; i<POPSIZE; i++)
	{
		population[i] = temp[i];
	}

	return 0;
}

