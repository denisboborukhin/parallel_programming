CC = mpic++ -std=c++14

SRC = ./src/

all: exponent

exponent: $(SRC)exponent.cpp
	$(CC) -o exponent $^
	
run_exponent: exponent
	mpirun -np 4 ./exponent 7

clean:
	rm -rf exponent 