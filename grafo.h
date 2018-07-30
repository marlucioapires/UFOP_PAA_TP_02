#ifndef GRAFO_H_INCLUDED
#define GRAFO_H_INCLUDED

#include "lista_encadeada.h"

typedef struct {
    int **matriz_adj;
    int num_vertices;
    int *graus_vertices; // Vetor que armazena os graus de cada um dos vértices do grafo.
    int *vetor_indices; // Vetor usado para ordenação indireta dos vértices do grafo.
} tipo_grafo;

typedef tipo_grafo *grafo;

grafo cria_grafo(unsigned int);
int adiciona_aresta(grafo, int, int);
void termina_grafo(grafo);
int tamanho_grafo(grafo);
int posicao_real_vertice(grafo, int);
void imprime_grafo(grafo);
int pesquisa_adjascente(grafo, int, lista);
int pesquisa_adjascente_exceto(grafo, int, lista, int);

void ordena(grafo, int*);

#endif // GRAFO_H_INCLUDED
