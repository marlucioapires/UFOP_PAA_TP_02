#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"

grafo cria_grafo(unsigned int tam)
{
    int i, j;

    grafo g = (grafo) malloc(sizeof(tipo_grafo));

    if(g) {
        g->graus_vertices = (int*) malloc(sizeof(int) * tam);
        if(!g->graus_vertices) {
            free(g);
            return NULL;
        }

        for(j = 0; j < tam; j++)
            g->graus_vertices[j] = 0;

        g->vetor_indices = (int*) malloc(sizeof(int) * tam);
        if(!g->graus_vertices) {
            free(g->graus_vertices);
            free(g);
            return NULL;
        }

        for(j = 0; j < tam; j++)
            g->vetor_indices[j] = j;

        g->matriz_adj = (int**) malloc(sizeof(int*) * tam);
        if(g->matriz_adj) {
            for (i = 0; i < tam; i++) {
                g->matriz_adj[i] = (int*) malloc(sizeof(int) * tam);
                if (!g->matriz_adj[i]) {
                    g->num_vertices = i;
                    termina_grafo(g);
                    return NULL;
                } else {
                    for(j = 0; j < tam; j++)
                        g->matriz_adj[i][j] = 0;
                }
            }
            g->num_vertices = tam;
        } else {
            free(g->graus_vertices);
            free(g->vetor_indices);
            free(g);
            g = NULL;
        }
    }
    return g;
}

int adiciona_aresta(grafo g, int vert1, int vert2)
{
    if (!g->matriz_adj[vert1][vert2] && vert1 != vert2) {
        g->matriz_adj[vert1][vert2] = 1;
        g->graus_vertices[vert1]++;
        g->matriz_adj[vert2][vert1] = 1;
        g->graus_vertices[vert2]++;
        return 1;
    } else {
        return 0;
    }
}

void termina_grafo(grafo g)
{
    int i;

    if(g) {
        free(g->graus_vertices);
        free(g->vetor_indices);
        for(i = 0; i < g->num_vertices; i++)
            free(g->matriz_adj[i]);
        free(g->matriz_adj);
        free(g);
    }
}

int tamanho_grafo(grafo g)
{
    return g->num_vertices;
}

int posicao_real_vertice(grafo g, int indice_vertice)
{
    return g->vetor_indices[indice_vertice];
}

void imprime_grafo(grafo g)
{
    int i, j;

    printf("\nMATRIZ DE ADJACENCIA DO GRAFO:\n\n");
    for(i = 0; i < g->num_vertices; i++) {
        for(j = 0; j < g->num_vertices; j++)
            printf(" %d ", g->matriz_adj[i][j]);
        printf("\n");
    }
    printf("\nGRAUS DOS VERTICES DO GRAFO:\n\n");
    for(i = 0; i < g->num_vertices; i++) {
        printf(" %d ", g->graus_vertices[i]);
    }
    printf("\n");
    printf("\nVETOR INDICES DO GRAFO:\n\n");
    for(i = 0; i < g->num_vertices; i++) {
        printf(" %d ", g->vetor_indices[i]);
    }
    printf("\n");
}

int pesquisa_adjascente(grafo g, int vert, lista lista_vertices)
{
    return pesquisa_indices_vetor_na_lista(lista_vertices, g->matriz_adj[vert]);
}

int pesquisa_adjascente_exceto(grafo g, int vert, lista lista_vertices, int vert_excluido)
{
    return pesquisa_indices_vetor_na_lista_exceto(lista_vertices, g->matriz_adj[vert], vert_excluido);
}

void merge(grafo g, int l, int m, int r, int* pesos_itens)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    int *vetor_aux1, *vetor_aux2;

    vetor_aux1 = (int*) malloc(sizeof(int) * n1);
    vetor_aux2 = (int*) malloc(sizeof(int) * n2);

    for (i = 0; i < n1; i++)
        vetor_aux1[i] = g->vetor_indices[l + i];
    for (j = 0; j < n2; j++)
        vetor_aux2[j] = g->vetor_indices[m + 1+ j];

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (g->graus_vertices[vetor_aux1[i]] > g->graus_vertices[vetor_aux2[j]]) {
            g->vetor_indices[k] = vetor_aux1[i];
            i++;
        } else if (g->graus_vertices[vetor_aux1[i]] < g->graus_vertices[vetor_aux2[j]]) {
            g->vetor_indices[k] = vetor_aux2[j];
            j++;
        } else {
            if (pesos_itens[vetor_aux1[i]] >= pesos_itens[vetor_aux2[j]]) {
                g->vetor_indices[k] = vetor_aux1[i];
                i++;
            } else {
                g->vetor_indices[k] = vetor_aux2[j];
                j++;
            }
        }
        k++;
    }

    while (i < n1) {
        g->vetor_indices[k] = vetor_aux1[i];
        i++;
        k++;
    }

    while (j < n2) {
        g->vetor_indices[k] = vetor_aux2[j];
        j++;
        k++;
    }

    free(vetor_aux1);
    free(vetor_aux2);
}

void mergeSort(grafo g, int l, int r, int* pesos_itens)
{
    if (l < r) {
        int m = l + (r - l) / 2;

        mergeSort(g, l, m, pesos_itens);
        mergeSort(g, m+1, r, pesos_itens);

        merge(g, l, m, r, pesos_itens);
    }
}

void ordena(grafo g, int* pesos_itens)
{
    mergeSort(g, 0, (g->num_vertices - 1), pesos_itens);
}

int eh_adjascente(grafo g, int vert1, int vert2) {
    return g->matriz_adj[vert1][vert2];
}

