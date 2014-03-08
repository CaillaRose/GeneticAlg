genetic: genetic.c
	gcc genetic.c -o genetic

dilemma: dilemma.c
	mpicc dilemma.c -o dilemma

all: genetic dilemma


genetic-test: genetic.c
	gcc genetic.c -o genetic;./genetic 100 1000 0.95 0.01

dilemma-test: dilemma.c
	mpicc dilemma.c -o dilemma;mpirun -n 4 ./dilemma 24 10 64 0.01 0.95

test: genetic-test dilemma-test


clean:
	rm dilemma genetic
