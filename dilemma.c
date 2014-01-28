/******************************************************
 *	Possible outcomes for each player:
 *	[R,S,T,P] = [3,0,5,1]
 *	R = Both Cooperate
 *	S = Sucker Score
 *	T = Temptation Score
 *	P = Both Defect
 *	Want to maximize the score = better fitness
 *	First games follow Nash Equilibrium:
 *	Both will defect
 * ***************************************************/

#include <stdio.h>
#include <stdlib.h>

#define NUM_GAMES 64
#define GENERATION 100
#define CROSSOVER 0.8
#define MUTATION 0.001
#define MAX_SCORE 6080

typedef struct
{
	int fitness;
	char decision[4];
}
pop;

pop** Allocate(int POPSIZE)
{
	pop **player = malloc(POPSIZE*sizeof(pop *));

	for (i=0; i<POPSIZE; i++)
	{
		player[i] = malloc(sizeof(pop));

		for(j=0; j<4; j++)
		{
			player[i].decision[j] = lrand48() % 2;
			player[i].fitness = 0;
		}
	}
	return player;
}
//0 == Defect; 1 == Cooperate
void Fitness(pop* player, pop* player)
{
	if (player[0].decision == 0 && player[1].decision == 0)
		player.fitness += 3;
	if (player[0].decision == 0 && player[1].decision == 1)
	{	
		player[0].fitness += 5;
		player[1].fitness += 0;
	}	
	if (player[0].decision == 1 && player[1].decision == 0)
	{
		player[0].fitness += 0;
		player[1].fitness += 5;
	}
	if (player[0].decision == 1 && player[1].decision == 1)
	{
		player[0].fitness += 1;
		player[1].fitness += 1;
	}

}

pop* Selection(pop** player)
{
	float sumFitness = 0.0f;
	int i;
	float RANDOM = (float)((lrand48()%1000)/999.0f)*sumFitness;

	for (i=0; i<POPSIZE; i++)
	{
		sumFitness += player[i].fitness;
		if (sumFitness >= RANDOM)
		{
			return player[i];
			break;
		}
	}
}

void Crossover(pop* player, pop* player)
{
	int temp1, temp2;



}

void Mutation(pop* player)
{
	RANDOM = drand48();
	if (RANDOM < MUTATION)
	{
		int mp = drand48() * 4;
		if (player.decision[mp] == 0) 
			player.decision[mp] = 1;
		else 
			player.decision[mp] = 0;
	}
	else break;
}

int main(int argc, char *argv[])
{
	int POPSIZE = 20;	//REPLACE WITH ARGV[1]
	int i,j,k,q;
	unsigned int seed = 87456;
	srand48(seed);

	Allocate(POPSIZE);
	//struct pop ***p = &player;
	
	for (k=0; k<GENERATION; k++)
	{
		for (q=0; q<NUM_GAMES; q++)
		{
			for (i=0; i<POPSIZE; i++)
			{
				for (j=POPSIZE-1; j>=0; j--)
				{
					player[i];	
					player[j];
					
					//Play Games 1 and 2
					//Play NUM_GAMES
					//Determine fitness and Store
				}
			}	
		}
		Selection(*prisoner[]);
		Crossover(players);
		Mutate(players);
		Do it all again!!
		
	}

	for (i=0; i<POPSIZE; i++)
	{
		free(player[i]);
	}
	return 0;
}
