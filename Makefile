CFLAGS = -Wall -g

all: tpgraph

tpgraph: main.o pixel.o
	g++ $(CFLAGS) -o tpgraph main.o pixel.o

main.o: main.cpp pixel.h
	g++ $(CFLAGS) -c main.cpp

pixel.o: pixel.cpp pixel.h
	g++ $(CFLAGS) -c pixel.cpp

clean:
	rm -f *.o tpgraph
