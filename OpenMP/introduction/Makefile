CC = clang 

CFLAGS 	?= -fopenmp 

SRC = ./src/

all: hello_world sum

hello_world: $(SRC)hello_world.c
	$(CC) $(CFLAGS) -o hello_world $^
	
run_hello_world: hello_world
	./hello_world

sum: $(SRC)sum.c
	$(CC) $(CFLAGS) -o sum $^
	
run_sum: sum
	./sum 10

sync: $(SRC)sync.c
	$(CC) $(CFLAGS) -o sync $^
	
run_sync: sync
	./sync

clean:
	rm -rf hello_world sum sync
