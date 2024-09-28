CC = mpic++ -std=c++11

SRC = ./src/

all: send_recv

send_recv: $(SRC)send_recv.cpp
	$(CC) -o send_recv $^
	
run_send_recv: send_recv
	mpirun -np 2 ./send_recv

clean:
	rm -rf send_recv