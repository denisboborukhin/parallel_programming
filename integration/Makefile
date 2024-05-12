CC = g++

SRC = ./src/

.PHONY: all
all: integration

integration: $(SRC)integration.cpp
	$(CC) -o $@ $^
	
clean:
	rm -rf integration out.csv