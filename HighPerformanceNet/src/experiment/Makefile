all : server client

server : server.o nettools.o
	g++ -o server server.o nettools.o -g
client : client.o nettools.o
	g++ -o client client.o nettools.o -g

nettools.o : nettools.cpp nettools.h
	g++ -c nettools.cpp -g

server.o : server.cpp nettools.o nettools.h
	g++ -c server.cpp nettools.o -g

client.o : client.cpp nettools.o nettools.h
	g++ -c client.cpp nettools.o -g

clean :
	rm server.o nettools.o client.o