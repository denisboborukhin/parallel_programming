CC = clang++

CFLAGS 	?= -fopenmp 

SRC = ./src/

all: balancer 

balancer: $(SRC)balancer.cpp
	$(CC) $(CFLAGS) -o balancer $^
	
run_balancer: balancer
	./balancer

clean:
	rm -rf balancer
