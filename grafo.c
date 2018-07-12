#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"

grafo cria_grafo(unsigned int t)
{
    int i, j;

    grafo g = (grafo)malloc(sizeof(tipo_grafo));
    if(g) {
        g->vertices = (lista*) malloc(sizeof(lista) * t);
        if(g->vertices) {
            for (i = 0; i < t; i++) {
                g->vertices[i] = cria_lista();
                if (!g->vertices[i]) {
                    for(j = 0; j < i; j++)
                        termina_lista(g->vertices[j]);
                    free(g->vertices);
                    free(g);
                    return NULL;
                }
            }
            g->num_vertices = t;
        } else {
            free(g);
            g = NULL;
        }
    }
    return g;
}

int adiciona_aresta(grafo g, tipo_chave ch1, tipo_chave ch2)
{
    tipo_elemento e;
    e.chave = ch2;
    insere_apos(g->vertices[ch1], e);
    e.chave = ch1;
    insere_apos(g->vertices[ch2], e);
    return 1;
}

void termina_grafo(grafo g)
{
    int i;

    if(g) {
        for(i = 0; i < g->num_vertices; i++)
            termina_lista(g->vertices[i]);
        free(g->vertices);
        free(g);
    }
}

void imprime_grafo(grafo g)
{
    int i;

    printf("\nLISTA DE ADJACENCIA DO GRAFO:\n");
    for(i = 0; i < g->num_vertices; i++) {
        printf("\n %d: ", (i + 1));
        imprime_lista(g->vertices[i]);
        printf("\n");
    }
}

int pesquisa_adjascente(grafo g, tipo_chave ch1, lista l)
{
    tipo_nodo *p;

    p = l->first;
    while(p) {
        if (pesquisa_chave(g->vertices[p->item.chave], ch1))
            return 1;
        p = p->next;
    }

    return 0;
}

int pesquisa_adjascente_exceto(grafo g, tipo_chave ch1, lista l, tipo_chave ch2)
{
    tipo_nodo *p;

    p = l->first;
    while(p) {
        if (p->item.chave != ch2) {
            if (pesquisa_chave(g->vertices[p->item.chave], ch1))
                return 1;
        }
        p = p->next;
    }

    return 0;
}

void merge(grafo g, int l, int m, int r, int* vetor_ind, int* pesos_itens)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    int *vetor_aux1, *vetor_aux2;

    vetor_aux1 = (int*) malloc(sizeof(int) * n1);
    vetor_aux2 = (int*) malloc(sizeof(int) * n2);

    for (i = 0; i < n1; i++)
        vetor_aux1[i] = vetor_ind[l + i];
    for (j = 0; j < n2; j++)
        vetor_aux2[j] = vetor_ind[m + 1+ j];

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (tamanho_lista(g->vertices[vetor_aux1[i]]) > tamanho_lista(g->vertices[vetor_aux2[j]])) {
            vetor_ind[k] = vetor_aux1[i];
            i++;
        } else if (tamanho_lista(g->vertices[vetor_aux1[i]]) < tamanho_lista(g->vertices[vetor_aux2[j]])) {
            vetor_ind[k] = vetor_aux2[j];
            j++;
        } else {
            if (pesos_itens[vetor_aux1[i]] >= pesos_itens[vetor_aux2[j]]) {
                vetor_ind[k] = vetor_aux1[i];
                i++;
            } else {
                vetor_ind[k] = vetor_aux2[j];
                j++;
            }
        }
        k++;
    }

    while (i < n1) {
        vetor_ind[k] = vetor_aux1[i];
        i++;
        k++;
    }

    while (j < n2) {
        vetor_ind[k] = vetor_aux2[j];
        j++;
        k++;
    }

    free(vetor_aux1);
    free(vetor_aux2);
}

void mergeSort(grafo g, int l, int r, int* vetor_ind, int* pesos_itens)
{
    if (l < r) {
        int m = l + (r - l) / 2;

        mergeSort(g, l, m, vetor_ind, pesos_itens);
        mergeSort(g, m+1, r, vetor_ind, pesos_itens);

        merge(g, l, m, r, vetor_ind, pesos_itens);
    }
}

void ordena(grafo g, int tam, int* vetor_ind, int* pesos_itens)
{
    int i;

    for(i = 0; i < tam; i++)
        vetor_ind[i] = i;
    mergeSort(g, 0, (tam - 1), vetor_ind, pesos_itens);
}

