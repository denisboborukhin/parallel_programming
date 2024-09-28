CC = clang 

CFLAGS 	?= -fopenmp 

SRC = ./src/

all: hello_world sum

hello_world: $(SRC)hello_world.c
	$(CC) -o hello_world $^
	
run_hello_world: hello_world
	./hello_world

clean:
	rm -rf hello_world sum ring
