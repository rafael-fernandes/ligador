CC = g++
LIBS = -std=c++11 -Wall
BIN = bin
INCLUDE = include
OBJ = obj
SRC = src
PROCESSED = processed
TMP = tmp

all: error_checker.o symbol.o assembler.o pre-processor.o macro-processor.o instructions.o main

error_checker.o: $(SRC)/error_checker.cpp $(INCLUDE)/error_checker.hpp
	$(CC) $(LIBS) -c $(SRC)/error_checker.cpp -I $(INCLUDE) -o $(OBJ)/error_checker.o

symbol.o: $(SRC)/symbol.cpp $(INCLUDE)/symbol.hpp
	$(CC) $(LIBS) -c $(SRC)/symbol.cpp -I $(INCLUDE) -o $(OBJ)/symbol.o

assembler.o: $(SRC)/assembler.cpp $(INCLUDE)/assembler.hpp
	$(CC) $(LIBS) -c $(SRC)/assembler.cpp -I $(INCLUDE) -o $(OBJ)/assembler.o

macro-processor.o: $(SRC)/macro-processor.cpp $(INCLUDE)/macro-processor.hpp
	$(CC) $(LIBS) -c $(SRC)/macro-processor.cpp -I $(INCLUDE) -o $(OBJ)/macro-processor.o

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
	rm $(TMP)/*