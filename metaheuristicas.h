#ifndef METAHEURISTICAS_H_INCLUDED
#define METAHEURISTICAS_H_INCLUDED

#include <time.h>
#include "grafo.h"
#include "lista_encadeada.h"

#define ERRO -1

typedef struct tupla {
    int c1, c2, i1, i2;
} tipo_tupla;

int simulated_annealing(int*, int, grafo, lista*, int*, int, double, double, double, clock_t, long int);

#endif // METAHEURISTICAS_H_INCLUDED
