#include <stdlib.h>

#include "algoritmos_gulosos.h"
#include "lista_encadeada.h"

int firstFit_conflitante(int* weight, int n, int c, grafo g, int* vetor_ind, lista**conteineres, int**vetor_sobras_bins)
{
    int res = 0;

    tipo_elemento e;

    int* bin_rem = (int*) malloc(sizeof(int) * n);

    lista* bin_itens = (lista*) malloc(sizeof(lista) * n);

    for (int i = 0; i < n; i++) {
        int j;
        for (j = 0; j < res; j++) {
            if (bin_rem[j] >= weight[vetor_ind[i]]) {
                if (pesquisa_adjascente(g, vetor_ind[i], bin_itens[j]) == 0) {
                    bin_rem[j] = bin_rem[j] - weight[vetor_ind[i]];
                    e.chave = vetor_ind[i];
                    insere_apos(bin_itens[j], e);
                    break;
                }
            }
        }

        if (j==res) {
            bin_rem[res] = c - weight[vetor_ind[i]];
            bin_itens[j] = cria_lista();
            e.chave = vetor_ind[i];
            insere_apos(bin_itens[j], e);
            res++;
        }
    }
    *vetor_sobras_bins = bin_rem;
    *conteineres = bin_itens;
    return res;
}

int bestFit_conflitante(int* weight, int n, int c, grafo g, int* vetor_ind, lista**conteineres, int**vetor_sobras_bins)
{
    int res = 0;

    tipo_elemento e;

    int* bin_rem = (int*) malloc(sizeof(int) * n);

    lista* bin_itens = (lista*) malloc(sizeof(lista) * n);

    for (int i = 0; i < n; i++) {
        int j;

        int min = c + 1, bi = 0;

        for (j = 0; j < res; j++) {
            if (bin_rem[j] >= weight[vetor_ind[i]] && bin_rem[j] - weight[vetor_ind[i]] < min) {
                if (pesquisa_adjascente(g, vetor_ind[i], bin_itens[j]) == 0) {
                    bi = j;
                    min = bin_rem[j] - weight[vetor_ind[i]];
                }
            }
        }

        if (min == (c + 1)) {
            bin_rem[res] = c - weight[vetor_ind[i]];
            bin_itens[res] = cria_lista();
            e.chave = vetor_ind[i];
            insere_apos(bin_itens[res], e);
            res++;
        } else {
            e.chave = vetor_ind[i];
            insere_apos(bin_itens[bi], e);
            bin_rem[bi] -= weight[vetor_ind[i]];
        }
    }
    *vetor_sobras_bins = bin_rem;
    *conteineres = bin_itens;
    return res;
}

int worstFit_conflitante(int* weight, int n, int c, grafo g, int* vetor_ind, lista**conteineres, int**vetor_sobras_bins)
{
    int res = 0;

    tipo_elemento e;

    int* bin_rem = (int*) malloc(sizeof(int) * n);

    lista* bin_itens = (lista*) malloc(sizeof(lista) * n);

    for (int i = 0; i < n; i++) {
        int j;

        int max = -1, bi = 0;

        for (j = 0; j < res; j++) {
            if ((bin_rem[j] >= weight[vetor_ind[i]]) && (bin_rem[j] - weight[vetor_ind[i]] > max)) {
                if (pesquisa_adjascente(g, vetor_ind[i], bin_itens[j]) == 0) {
                    bi = j;
                    max = bin_rem[j] - weight[vetor_ind[i]];
                }
            }
        }

        if (max == -1) {
            bin_rem[res] = c - weight[vetor_ind[i]];
            bin_itens[res] = cria_lista();
            e.chave = vetor_ind[i];
            insere_apos(bin_itens[res], e);
            res++;
        } else {
            e.chave = vetor_ind[i];
            insere_apos(bin_itens[bi], e);
            bin_rem[bi] -= weight[vetor_ind[i]];
        }
    }
    *vetor_sobras_bins = bin_rem;
    *conteineres = bin_itens;
    return res;
}

