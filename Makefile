genetic: genetic.c
	gcc genetic.c -o genetic

dilemma: dilemma.c
	mpicc dilemma.c -o dilemma

all: genetic dilemma

dilemma-test: dilemma.c
	mpicc dilemma.c -o dilemma;mpirun -n 4 ./dilemma 20 100 64 0.01 0.95

test: dilemma-test


clean:
	rm dilemma genetic
