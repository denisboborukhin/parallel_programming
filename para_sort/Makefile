CC = mpic++ -std=c++11

SRC = ./src/

all: para_sort

para_sort: $(SRC)para_sort.cpp
	$(CC) -o para_sort $^
	
run_para_sort: para_sort 
	mpirun -np 2 ./para_sort 9

clean:
	rm -rf para_sort