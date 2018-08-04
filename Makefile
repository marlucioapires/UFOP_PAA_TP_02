# compilers
CC=gcc
CXX=g++

# flags
# debug
CFLAGS=-Og -std=c99 -Wall -g
CXXFLAGS=-Og -g

# opt
#CFLAGS=-Ofast -s -flto
#CXXFLAGS=-Ofast -s -flto

all:./bin/bpc 

./bin/bpc:./bin/bpc.o
	$(CXX) $(CXXFLAGS) ./bin/bpc.o ./bin/algoritmos_gulosos.o ./bin/funcoes.o ./bin/grafo.o ./bin/metaheuristicas.o ./bin/lista_encadeada.o -o ./bin/bpc

clean:
	mkdir -p ./bin ; rm ./bin/*.o

./bin/lista_encadeada.o:lista_encadeada.c lista_encadeada.h
	mkdir -p ./bin ; $(CC) -c $(CFLAGS) lista_encadeada.c -o ./bin/lista_encadeada.o

./bin/grafo.o:./grafo.c grafo.h ./bin/lista_encadeada.o
	mkdir -p ./bin ; $(CC) -c $(CFLAGS)  grafo.c  -o ./bin/grafo.o

./bin/bpc.o:./bpc.c ./bin/algoritmos_gulosos.o ./bin/funcoes.o ./bin/grafo.o ./bin/metaheuristicas.o
	mkdir -p ./bin ; $(CC) -c $(CFLAGS) bpc.c -o ./bin/bpc.o

./bin/algoritmos_gulosos.o:./algoritmos_gulosos.c algoritmos_gulosos.h ./bin/lista_encadeada.o
	mkdir -p ./bin ; $(CC) -c $(CFLAGS) algoritmos_gulosos.c -o ./bin/algoritmos_gulosos.o

./bin/funcoes.o:./funcoes.c funcoes.h
	mkdir -p ./bin ; $(CC) -c $(CFLAGS) funcoes.c -o ./bin/funcoes.o

./bin/metaheuristicas.o:./metaheuristicas.c metaheuristicas.h ./bin/funcoes.o
	mkdir -p ./bin ; $(CC) -c $(CFLAGS) metaheuristicas.c -o ./bin/metaheuristicas.o

