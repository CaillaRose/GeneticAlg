/*-------------------------------------------------------------------
 *	Possible outcomes for each player:
 *	[R,S,T,P] = [3,0,5,1]
 *	R = Both Cooperate
 *	S = Sucker Score
 *	T = Temptation Score
 *	P = Both Defect
 *	Want to maximize the score = better fitness
 *	Strategy is mix between Pavlov Theory and 
--------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <malloc.h>

typedef struct
{
	unsigned int history[4];
	unsigned int fitness;
	unsigned int move;
}
pop;

/*---------------------0 == Defect; 1 == Cooperate---------------*/

void Fitness(pop p[2])
{
	if (p[0].move == 0 && p[1].move == 0)
	{
		p[0].fitness += 3;
		p[1].fitness += 3;
	}
	else if (p[0].move == 0 && p[1].move == 1)
	{	
		p[0].fitness += 5;
		p[1].fitness += 0;
	}	
	else if (p[0].move == 1 && p[1].move == 0)
	{
		p[0].fitness += 0;
		p[1].fitness += 5;
	}
	else if (p[0].move == 1 && p[1].move == 1)
	{
		p[0].fitness += 1;
		p[1].fitness += 1;
	}
	else 
		return;
}

/*-----------------Strategy, based on prev 2 games------------------------*/
void Strategy(pop p, int total)
{
	switch(total)
	{
		case  0: p.move = 1; break;
		case  1: p.move = 0; break;
		case  2: p.move = 0; break;
		case  3: p.move = 0; break;
		case  4: p.move = 1; break;
		case  5: p.move = 1; break;
		case  6: p.move = 1; break;
		case  7: p.move = 0; break;
		case  8: p.move = 1; break;
		case  9: p.move = 0; break;
		case 10: p.move = 0; break;
		case 11: p.move = 0; break;
		case 12: p.move = 0; break;
		case 13: p.move = 0; break;
		case 14: p.move = 1; break;
		case 15: p.move = 1; break;
	}
}

/*---------------------------------------------------------------------
 *				
 *			MAIN FUNCTION
 *
 * -------------------------------------------------------------------*/

int main(int argc, char *argv[])
{
	MPI_Init(&argc, &argv);

	int POPSIZE = atoi(argv[1]);	
	int GENERATION = atoi(argv[2]);
	int NUM_GAMES = atoi(argv[3]);
	float CROSSOVER = atof(argv[4]);
	float MUTATION = atof(argv[5]);
	int i,j,k,q,s,b2d,count;
	int world_rank,world_size;
	unsigned int temp[2], temp2[2];
	float RANDOM2 = drand48();

	srand48(time(NULL));
	pop *player = NULL;
	pop p[2];

	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	
/*------------------------MPI_STRUCT----------------------------------*/
	MPI_Datatype mpi_pop;
	MPI_Datatype types[3] = {MPI_UNSIGNED,MPI_UNSIGNED,MPI_UNSIGNED};
	int block[3] = {4,1,1};
	MPI_Aint offset[3] = {offsetof(pop,history),offsetof(pop,fitness),offsetof(pop,move)};
	MPI_Type_create_struct(3,block,offset,types,&mpi_pop);
	MPI_Type_commit(&mpi_pop);

	if (world_rank == 0)
	{
		if (world_size > POPSIZE/4)
		{
			printf("Too many processes for Population Size.\n Please input an even Population Size.\n");
			MPI_Abort(MPI_COMM_WORLD, 1);
		}
	}

/*---------------Allocate the memory for the players------------------*/
	
	if (world_rank == 0)
	{	
		player = malloc(POPSIZE*sizeof(pop));
		for (i=0; i<POPSIZE; i++)
		{
			for(j=0; j<4; j++)
			{
				player[i].history[j] = lrand48() % 2;
				player[i].fitness = 0;
			}
		}	
		printf("Processor %d has data:\n", world_rank);
		for (i=0; i<POPSIZE; i++)
		{
			for (j=0; j<4; j++)
			{
				printf("%d ", player[i].history[j]);
			}
			printf("\n");
		}
	}
	
	int ARRAY_SIZE = POPSIZE/4;
	pop *sub_arrays = NULL;

	if (world_rank != 0)
	{
		sub_arrays = malloc(ARRAY_SIZE*sizeof(pop));
	}

/*-----------------------RUN THE ALGORITHM---------------------------*/	

	for (k=0; k<GENERATION; k++)
	{
		MPI_Scatter(player, ARRAY_SIZE, mpi_pop, sub_arrays, ARRAY_SIZE, mpi_pop, 0, MPI_COMM_WORLD);

		if (world_rank != 0)
		{
			for (i=0; i<ARRAY_SIZE; i++)
			{
				for (j=ARRAY_SIZE; j>=0; j--)
				{
					p[0] = player[i];	
					p[1] = player[j];		
				
					for(q=0; q<NUM_GAMES; q++)
					{
						b2d = ((p[0].history[0]*8) + (p[0].history[1]*4) + (p[0].history[2]*2) + p[0].history[3]);
						b2d = ((p[1].history[0]*8) + (p[1].history[1]*4) + (p[1].history[2]*2) + p[1].history[3]);
									
						Strategy(p[0], b2d);
						Strategy(p[1], b2d);
						Fitness(p);

						for (s=4; s>0; s--)
						{
							p[0].history[s] = p[0].history[s-1];
							p[1].history[s] = p[1].history[s-1];
						} 	
						p[0].history[0] = p[0].move;
						p[1].history[0] = p[1].move;
					}

					player[i] = p[0];
					player[j] = p[1];
				}
			}
		}

		MPI_Barrier(MPI_COMM_WORLD);
		MPI_Gather(sub_arrays, ARRAY_SIZE, mpi_pop, player, ARRAY_SIZE, mpi_pop, 0, MPI_COMM_WORLD);	

/*-----------------------Perform Selection-----------------------------*/		
	
		if (world_rank == 0)
		{
			for(count=0; count<2; count++)
			{
				int sumFitness = 0;
				for (i=0; i<POPSIZE; i++)
				{
					sumFitness += p[i].fitness;
					int RANDOM = lrand48() % sumFitness;
					if (sumFitness >= RANDOM)
					{
						p[count] =  player[i];
					}
				}
			}
/*------------------------Crossover-------------------------------------*/

			if (RANDOM2 < CROSSOVER)
			{
				temp [0] = p[0].history[2];
				temp [1] = p[0].history[3];
				temp2[0] = p[1].history[2];
				temp2[1] = p[1].history[3];
				p[0].history[2] = temp2[0];
				p[0].history[3] = temp2[1];
				p[1].history[2] = temp[ 0];
				p[1].history[3] = temp[ 1];
			}

/*---------------------Mutate Players------------------------------------*/


			if (RANDOM2 < MUTATION)
			{
				int mp = lrand48() % 4;
				for (count=0; count<2; count++)
				{
					if (p[count].history[mp] == 0) 
					{	
						p[count].history[mp] = 1;
					}
					else
					{ 
						p[count].history[mp] = 0;
					}
				}
			}
			
			player[lrand48() % POPSIZE] = p[0];
			player[lrand48() % POPSIZE] = p[1];
		}

		printf("Processor %d has data:\n", world_rank);
		for (i=0; i<POPSIZE; i++)
		{
			for (j=0; j<4; j++)
			{
				printf("%d ", player[i].history[j]);
			}
			printf("\n");
		}
	}

	for(i=0; i<POPSIZE; i++)
	{
		for (j=0; j<4; j++)
		{
			printf("%d", player[i].history[j]);
		}
		printf("\n");
		printf("Fitness: %d\n", player[i].fitness);
	}
	if (world_rank == 0)
		free(player);
	if (world_rank != 0)
		free(sub_arrays);

	MPI_Finalize();
	return 0;

}

