all: main

main: huff.o main.o
	g++ -o main huff.o main.o

huff.o: huff.h huff.cpp
	g++ -c huff.cpp

main.o: main.cpp
	g++ -c main.cpp

clean:
	rm -f *.o main
