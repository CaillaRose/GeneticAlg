/*--------------------------------------------------------------------
 *	Possible outcomes for each player:
 *	[R,S,T,P] = [3,0,5,1]
 *	R = Both Cooperate
 *	S = Sucker Score
 *	T = Temptation Score
 *	P = Both Defect
 *	Want to maximize the score = better fitness
 *	Strategy is based on Pavlov Theory
--------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_GAMES 64
#define GENERATION 100
#define CROSSOVER 0.95
#define MUTATION 0.01
#define MAX_SCORE 6080

typedef struct
{
	unsigned int fitness;
	unsigned int move;
	char history[4];
}
pop;

//0 == Defect; 1 == Cooperate
void Fitness(pop* p1, pop* p2)
{
	if (p1->move == 0 && p2->move == 0)
	{
		p1->fitness += 3;
		p2->fitness += 3;
	}
	else if (p1->move == 0 && p2->move == 1)
	{	
		p1->fitness += 5;
		p2->fitness += 0;
	}	
	else if (p1->move == 1 && p2->move == 0)
	{
		p1->fitness += 0;
		p2->fitness += 5;
	}
	else if (p1->move == 1 && p2->move == 1)
	{
		p1->fitness += 1;
		p2->fitness += 1;
	}
	else 
		return;
}

/*---------------Strategy, based on prev 2 games------------------------*/
void Strategy(pop* p, int total)
{
	switch(total)
	{
		case  0: p->move = 1;
		case  1: p->move = 0;
		case  2: p->move = 0;
		case  3: p->move = 0;
		case  4: p->move = 1;
		case  5: p->move = 1;
		case  6: p->move = 1;
		case  7: p->move = 0;
		case  8: p->move = 1;
		case  9: p->move = 0;
		case 10: p->move = 0;
		case 11: p->move = 0;
		case 12: p->move = 0;
		case 13: p->move = 0;
		case 14: p->move = 1;
		case 15: p->move = 1;
	}
}

/*---------------------------------------------------------------------
 *				
 *			MAIN FUNCTION
 *
 * -------------------------------------------------------------------*/

int main(int argc, char *argv[])
{
	int POPSIZE = atoi(argv[1]);	//REPLACE WITH ARGV[1]
	int i,j,k,q,s,b2d;
	//unsigned long int seed = 29381742375;
	srand48(time(NULL));
	pop* p1;
	pop* p2;

/*---------------Allocate the memory for the players------------------*/

	pop **player = malloc(POPSIZE*sizeof(pop *));

	for (i=0; i<POPSIZE; i++)
	{
		player[i] = malloc(sizeof(pop));
	}

/*-----------------------RUN THE ALGORITHM---------------------------*/	

	for (k=0; k<GENERATION; k++)
	{
		for (i=0; i<POPSIZE; i++)
		{
			for(j=0; j<4; j++)
			{
				player[i]->history[j] = lrand48() % 2;
				player[i]->fitness = 0;
			}
		}

		for (i=0; i<POPSIZE; i++)
		{
			for (j=POPSIZE-1; j>=0; j--)
			{
				p1 = player[i];	
				p2 = player[j];		
				for(q=0; q<NUM_GAMES; q++)
				{
					//Convert the binary value to decimal and then calculate the move
					b2d = p1->history[0]*8 + p1->history[1]*4 + p1->history[2]*2 + p1->history[3];
					Strategy(p1, b2d);
					b2d = p2->history[0]*8 + p2->history[1]*4 + p2->history[2]*2 + p2->history[3];
					Strategy(p2, b2d);
					Fitness(p1, p2);
					for (s=4; s>=0; s--)
					{
						p1->history[s] = p1->history[s-1];
						p2->history[s] = p2->history[s-1];
					} 	
					p1->history[0] = p1->move;
					p2->history[0] = p2->move;
				}
			}
		}	
		
/*-----------------------Perform Selection-----------------------------*/		
		pop* parent[2];
		int count;

		for(count=0; count<2; count++)
		{
			int sumFitness = 0;
			for (i=0; i<POPSIZE; i++)
			{
				sumFitness += player[i]->fitness;
				int RANDOM = lrand48() % sumFitness;
				if (sumFitness >= RANDOM)
				{
					parent[count] =  player[i];
				}
			}
		}
/*------------------------Crossover-------------------------------------*/
	
		float RANDOM2 = drand48();
		int temp[2], temp2[2];
		if (RANDOM2 < CROSSOVER)
		{
			temp[ 0] = parent[0]->history[2];
			temp[ 1] = parent[0]->history[3];
			temp2[0] = parent[1]->history[2];
			temp2[1] = parent[1]->history[3];
			parent[0]->history[2] = temp2[0];
			parent[0]->history[3] = temp2[1];
			parent[1]->history[2] = temp[ 0];
			parent[1]->history[3] = temp[ 1];
		}

/*---------------------Mutate Players------------------------------------*/


		if (RANDOM2 < MUTATION)
		{
			int mp = lrand48() % 4;
			for (count=0; count<2; count++)
			{
				if (parent[count]->history[mp] == 0) 
					parent[count]->history[mp] = 1;
				else 
					parent[count]->history[mp] = 0;
			}
		}
	}

	for(i=0; i<POPSIZE; i++)
	{
		for (j=0; j<4; j++)
		{
			printf("%d", player[i]->history[j]);
		}
		printf("\n");
		printf("Fitness: %d\n", player[i]->fitness);
	}

	for (i=0; i<POPSIZE; i++)
	{
		free(player[i]);
	}
	return 0;
}
