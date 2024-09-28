CC = mpic++ -std=c++11

SRC = ./src/

all: diffusion 

diffusion: $(SRC)solver.cpp $(SRC)main.cpp
	$(CC) -o diffusion $^
	
run_diffusion: diffusion
	mpirun -np 1 ./diffusion
	mpirun -np 2 ./diffusion
	mpirun -np 4 ./diffusion
	mpirun -np 6 ./diffusion

clean:
	rm -rf diffusion