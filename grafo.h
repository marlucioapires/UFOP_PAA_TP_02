#ifndef GRAFO_H_INCLUDED
#define GRAFO_H_INCLUDED

#include "lista_encadeada.h"

typedef struct {
    lista *vertices;
    int num_vertices/*, tamanho*/;
} tipo_grafo;

typedef tipo_grafo *grafo;

grafo cria_grafo(unsigned int);
int adiciona_aresta(grafo, tipo_chave, tipo_chave);
void termina_grafo(grafo);
void imprime_grafo(grafo);
int pesquisa_adjascente(grafo, tipo_chave, lista);
int pesquisa_adjascente_exceto(grafo, tipo_chave, lista, tipo_chave);

void ordena(grafo, int, int*, int*);

#endif // GRAFO_H_INCLUDED
