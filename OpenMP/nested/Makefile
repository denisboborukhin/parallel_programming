CC = clang++

CFLAGS 	?= -fopenmp 

SRC = ./src/

all: nested 

nested: $(SRC)nested.cpp
	$(CC) $(CFLAGS) -o nested $^
	
run_nested: nested
	./nested

clean:
	rm -rf nested
