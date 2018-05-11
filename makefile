CC = g++
LIBS = -std=c++11 -Wall

main:
	# clean
	$(CC) $(LIBS) main.cpp
	./a.out

clean:
	rm -f a.out
	rm -f main.o