CC = mpic++ -std=c++11

SRC = ./src/

all: para_sort sort

para_sort: $(SRC)para_sort.cpp
	$(CC) -o para_sort $^

sort: $(SRC)sort.cpp
	g++ -o sort $^
	
LENTH = 1000000
run: para_sort sort
	./sort $(LENTH) 
	mpirun -np 2 ./para_sort $(LENTH)
	mpirun -np 4 ./para_sort $(LENTH)
	mpirun -np 6 ./para_sort $(LENTH)
	mpirun -np 8 ./para_sort $(LENTH)
	mpirun -np 10 ./para_sort $(LENTH)
	mpirun -np 15 ./para_sort $(LENTH)
	mpirun -np 20 ./para_sort $(LENTH)

clean:
	rm -rf para_sort sort