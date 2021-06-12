all: client.cpp server.cpp 
	g++ -o server server.cpp
	g++ -o client client.cpp
clean: client server
	rm -f server client
	rm -rf recv*
test: server
	./server
