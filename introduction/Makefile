CC = mpicc

SRC = ./src/

all: hello_world sum

hello_world: $(SRC)hello_world.c
	$(CC) -o hello_world $^
	
run_hello_world: hello_world
	mpirun -np 4 ./hello_world

sum: $(SRC)sum.c
	$(CC) -o sum $^
	
run_sum: sum
	mpirun -np 4 ./sum 100

ring: $(SRC)ring.c
	$(CC) -o ring $^
	
run_ring: ring
	mpirun -np 5 ./ring

clean:
	rm -rf hello_world sum ring
