CC = g++
LIBS = -std=c++11 -Wall
BIN = bin
INCLUDE = include
OBJ = obj
SRC = src
PROCESSED = processed

all: symbol.o pre-processor.o instructions.o main

symbol.o: $(SRC)/symbol.cpp $(INCLUDE)/symbol.hpp
	$(CC) $(LIBS) -c $(SRC)/symbol.cpp -I $(INCLUDE) -o $(OBJ)/symbol.o

pre-processor.o: $(SRC)/pre-processor.cpp $(INCLUDE)/pre-processor.hpp
	$(CC) $(LIBS) -c $(SRC)/pre-processor.cpp -I $(INCLUDE) -o $(OBJ)/pre-processor.o

instructions.o: $(SRC)/instructions.cpp $(INCLUDE)/instructions.hpp
	$(CC) $(LIBS) -c $(SRC)/instructions.cpp -I $(INCLUDE) -o $(OBJ)/instructions.o

main:
	$(CC) $(LIBS) main.cpp $(OBJ)/*.o -I $(INCLUDE) -o $(BIN)/main

run:
	./$(BIN)/main

clean: 
	rm $(OBJ)/*.o
	rm $(BIN)/*
	rm $(PROCESSED)/*