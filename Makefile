# compilers
CC=gcc
CXX=g++

# flags
# debug
CFLAGS=-Og -std=c99 -Wall -g -DGRB -I/opt/gurobi800/linux64/include/
CXXFLAGS=-Og -g -DGRB -I/opt/gurobi800/linux64/include/
LDFLAGS=-L/opt/gurobi800/linux64/lib/ -Og -g
LIBRARIES=-lgurobi80 -lpthread


# opt
#CFLAGS=-Ofast -s -flto -DGRB
#CXXFLAGS=-Ofast -s -flto -DGRB
#LDFLAGS=-L/opt/gurobi800/linux64/lib/ -Ofast -flto -s

all:./bin/bpc 

./bin/bpc:./bin/bpc.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) ./bin/bpc.o ./bin/algoritmos_gulosos.o ./bin/funcoes.o \
	    ./bin/grafo.o ./bin/metaheuristicas.o ./bin/otim_mip.o ./bin/lp.o \
	    ./bin/lista_encadeada.o -o ./bin/bpc $(LIBRARIES)

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

./bin/metaheuristicas.o:./metaheuristicas.c metaheuristicas.h ./bin/funcoes.o ./bin/otim_mip.o
	mkdir -p ./bin ; $(CC) -c $(CFLAGS) metaheuristicas.c -o ./bin/metaheuristicas.o

./bin/lp.o:lp.cpp lp.h
	mkdir -p ./bin ; $(CXX) -c $(CFLAGS) lp.cpp -o ./bin/lp.o

./bin/otim_mip.o:otim_mip.c otim_mip.h ./bin/lp.o
	mkdir -p ./bin ; $(CC) -c $(CFLAGS) otim_mip.c -o ./bin/otim_mip.o

