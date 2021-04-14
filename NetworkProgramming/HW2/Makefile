all: client.cpp server.cpp huff/huff.cpp
	g++ -o client client.cpp huff/huff.cpp huff2/huff2.cpp
	g++ -o server server.cpp huff/huff.cpp huff2/huff2.cpp
clean:
	rm -f client server rev/* *-coded *-codebook 
