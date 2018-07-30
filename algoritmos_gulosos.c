#include <stdlib.h>

#include "algoritmos_gulosos.h"
#include "lista_encadeada.h"

int firstFit_conflitante(int* weight, int c, grafo g, lista**conteineres, int**vetor_sobras_bins)
{
    int res = 0;

    tipo_elemento e;

    int* bin_rem = (int*) malloc(sizeof(int) * tamanho_grafo(g));

    lista* bin_itens = (lista*) malloc(sizeof(lista) * tamanho_grafo(g));

    for (int i = 0; i < tamanho_grafo(g); i++) {
        int j;
        for (j = 0; j < res; j++) {
            if (bin_rem[j] >= weight[posicao_real_vertice(g, i)]) {
                if (pesquisa_adjascente(g, posicao_real_vertice(g, i), bin_itens[j]) == 0) {
                    bin_rem[j] = bin_rem[j] - weight[posicao_real_vertice(g, i)];
                    e.chave = posicao_real_vertice(g, i);
                    insere_apos(bin_itens[j], e);
                    break;
                }
            }
        }

        if (j==res) {
            bin_rem[res] = c - weight[posicao_real_vertice(g, i)];
            bin_itens[j] = cria_lista();
            e.chave = posicao_real_vertice(g, i);
            insere_apos(bin_itens[j], e);
            res++;
        }
    }
    *vetor_sobras_bins = bin_rem;
    *conteineres = bin_itens;
    return res;
}

int bestFit_conflitante(int* weight, int c, grafo g, lista**conteineres, int**vetor_sobras_bins)
{
    int res = 0;

    tipo_elemento e;

    int* bin_rem = (int*) malloc(sizeof(int) * tamanho_grafo(g));

    lista* bin_itens = (lista*) malloc(sizeof(lista) * tamanho_grafo(g));

    for (int i = 0; i < tamanho_grafo(g); i++) {
        int j;

        int min = c + 1, bi = 0;

        for (j = 0; j < res; j++) {
            if (bin_rem[j] >= weight[posicao_real_vertice(g, i)] && bin_rem[j] - weight[posicao_real_vertice(g, i)] < min) {
                if (pesquisa_adjascente(g, posicao_real_vertice(g, i), bin_itens[j]) == 0) {
                    bi = j;
                    min = bin_rem[j] - weight[posicao_real_vertice(g, i)];
                }
            }
        }

        if (min == (c + 1)) {
            bin_rem[res] = c - weight[posicao_real_vertice(g, i)];
            bin_itens[res] = cria_lista();
            e.chave = posicao_real_vertice(g, i);
            insere_apos(bin_itens[res], e);
            res++;
        } else {
            e.chave = posicao_real_vertice(g, i);
            insere_apos(bin_itens[bi], e);
            bin_rem[bi] -= weight[posicao_real_vertice(g, i)];
        }
    }
    *vetor_sobras_bins = bin_rem;
    *conteineres = bin_itens;
    return res;
}

int worstFit_conflitante(int* weight, int c, grafo g, lista**conteineres, int**vetor_sobras_bins)
{
    int res = 0;

    tipo_elemento e;

    int* bin_rem = (int*) malloc(sizeof(int) * tamanho_grafo(g));

    lista* bin_itens = (lista*) malloc(sizeof(lista) * tamanho_grafo(g));

    for (int i = 0; i < tamanho_grafo(g); i++) {
        int j;

        int max = -1, bi = 0;

        for (j = 0; j < res; j++) {
            if ((bin_rem[j] >= weight[posicao_real_vertice(g, i)]) && (bin_rem[j] - weight[posicao_real_vertice(g, i)] > max)) {
                if (pesquisa_adjascente(g, posicao_real_vertice(g, i), bin_itens[j]) == 0) {
                    bi = j;
                    max = bin_rem[j] - weight[posicao_real_vertice(g, i)];
                }
            }
        }

        if (max == -1) {
            bin_rem[res] = c - weight[posicao_real_vertice(g, i)];
            bin_itens[res] = cria_lista();
            e.chave = posicao_real_vertice(g, i);
            insere_apos(bin_itens[res], e);
            res++;
        } else {
            e.chave = posicao_real_vertice(g, i);
            insere_apos(bin_itens[bi], e);
            bin_rem[bi] -= weight[posicao_real_vertice(g, i)];
        }
    }
    *vetor_sobras_bins = bin_rem;
    *conteineres = bin_itens;
    return res;
}

