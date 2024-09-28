CC = mpic++ -std=c++11

SRC = ./src/

all: client server

server: $(SRC)server.cpp
	$(CC) -o server $^

client: $(SRC)client.cpp
	$(CC) -o client $^

master: $(SRC)master.cpp
	$(CC) -o master $^
	
run: master server client
	mpirun -np 1 ./master
	
clean:
	rm -rf server client master