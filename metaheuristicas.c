#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "metaheuristicas.h"
#include "funcoes.h"

void escolhe_itens_permuta(int*, int*, int, int*, int*, lista*);
unsigned long int computa_proxima_solucao(int*, int*, int, int*, int*, int*, int, grafo, lista*, int*, int);
unsigned long int funcao_para_comparacao_solucoes(int*, int, int, int, int, int, int);
float gera_probabilidade();
void elimina_bin(lista*, int*, int, int);
double calcula_segundos_transcorridos(clock_t);

int simulated_annealing(int *weight, int capacidade, grafo g, lista *bins_solucao, int *bin_rem,
                        int tam_solucao, float alpha, float temperatura, float epsilon, clock_t tempo_inicial,
                        long int segundos_tempo_maximo)
{

    int j, c1, c2, pos_i1, pos_i2, item1, item2, peso1, peso2, trocar_solucao;

    unsigned long sol1, sol2;

    long int lower_bound = 0;

    // Probabilidade.
    float prob, delta;

    if (alpha <= 0 || alpha >= 1)
        alpha = 0.999;

    for(j = 0; j < tamanho_grafo(g); j++)
        lower_bound += weight[j];

    if (temperatura <= 0) {
        temperatura = lower_bound * lower_bound;
    }

    lower_bound = ((float) lower_bound / capacidade);

    if (epsilon <= 0)
        epsilon = 0.001;

    inicia_gerador_numero_aleatorio();

    sol1 = funcao_para_comparacao_solucoes(bin_rem, tam_solucao, capacidade, 0, 0, 0, 0);

    while ((calcula_segundos_transcorridos(tempo_inicial) < segundos_tempo_maximo) && (temperatura > epsilon) &&
            tam_solucao > lower_bound) {

        sol2 = computa_proxima_solucao(&c1, &c2, tam_solucao, &pos_i1, &pos_i2, weight, capacidade, g, bins_solucao,
                                       bin_rem, tam_solucao);

        if (sol2 != ERRO) {

            delta = sol2 - sol1;

            trocar_solucao = 1;
            if (delta <= 0) {
                prob = gera_probabilidade();

                if (prob > exp(-delta/temperatura))
                    trocar_solucao = 0;
            }

            if (trocar_solucao) {
                sol1 = sol2;

                if (pos_i1 == -1) {

                    item2 = get_chave_posicao(bins_solucao[c2], pos_i2);

                    peso2 = weight[item2];

                    transfere_elemento(bins_solucao[c2], pos_i2, bins_solucao[c1]);

                    bin_rem[c1] -= peso2;
                    bin_rem[c2] += peso2;

                    if (bin_rem[c2] == capacidade) { // Conteiner está vazio
                        elimina_bin(bins_solucao, bin_rem, tam_solucao, c2);
                        tam_solucao--;
                    }
                } else if (pos_i2 == -1) {

                    item1 = get_chave_posicao(bins_solucao[c1], pos_i1);

                    peso1 = weight[item1];

                    transfere_elemento(bins_solucao[c1], pos_i1, bins_solucao[c2]);

                    bin_rem[c1] += peso1;
                    bin_rem[c2] -= peso1;

                    if (bin_rem[c1] == capacidade) { // Conteiner está vazio
                        elimina_bin(bins_solucao, bin_rem, tam_solucao, c1);
                        tam_solucao--;
                    }
                } else {

                    item1 = get_chave_posicao(bins_solucao[c1], pos_i1);
                    item2 = get_chave_posicao(bins_solucao[c2], pos_i2);

                    peso1 = weight[item1];
                    peso2 = weight[item2];

                    troca_chaves(bins_solucao[c1], pos_i1, bins_solucao[c2], pos_i2);

                    bin_rem[c1] += peso1 - peso2;
                    bin_rem[c2] += peso2 - peso1;
                }
            }
            temperatura *= alpha;
        }
    }
    return tam_solucao;
}

void escolhe_itens_permuta(int* c1, int* c2, int max_c, int* pos_i1, int* pos_i2, lista *bins_solucao)
{

    if (max_c > 1) {
        *c1 = gera_numero_aleatorio(max_c);
        do {
            *c2 = gera_numero_aleatorio(max_c);
        } while(*c1 == *c2);
        (*pos_i1) = gera_numero_aleatorio(tamanho_lista(bins_solucao[*c1])) + 1;
        (*pos_i2) = gera_numero_aleatorio(tamanho_lista(bins_solucao[*c2])) + 1;
    }
}

unsigned long int computa_proxima_solucao(int* c1, int* c2, int max_c, int* pos_i1, int* pos_i2,
        int *weight, int c, grafo g, lista *bins_solucao, int *bin_rem, int tam_solucao)
{
    int peso1, peso2, item1, item2, escolha;

    escolhe_itens_permuta(c1, c2, max_c, pos_i1, pos_i2, bins_solucao);

    item1 = get_chave_posicao(bins_solucao[(*c1)], *pos_i1);
    item2 = get_chave_posicao(bins_solucao[(*c2)], *pos_i2);

    peso1 = weight[item1];
    peso2 = weight[item2];

    escolha = gera_numero_aleatorio(2);

    if (escolha == 0) {
        if (bin_rem[*c2] - peso1 >= 0) { // Item pode ser transferido sem troca.
            if (pesquisa_adjascente(g, item1, bins_solucao[*c2]) == 0) {
                *pos_i2 = -1;
                return funcao_para_comparacao_solucoes(bin_rem, tam_solucao, c, *c1, *c2, peso1, 0);
            }
        } else if (bin_rem[*c1] - peso2 >= 0) { // Item pode ser transferido sem troca.
            if (pesquisa_adjascente(g, item2, bins_solucao[*c1]) == 0) {
                *pos_i1 = -1;
                return funcao_para_comparacao_solucoes(bin_rem, tam_solucao, c, *c1, *c2, 0, peso2);
            }
        }
    } else if ((bin_rem[*c1] + peso1 - peso2 >= 0) && (bin_rem[*c2] + peso2 - peso1 >= 0)) {
        if ((pesquisa_adjascente_exceto(g, item1, bins_solucao[*c2], item2) == 0) &&
                (pesquisa_adjascente_exceto(g, item2, bins_solucao[*c1], item1) == 0))
            return funcao_para_comparacao_solucoes(bin_rem, tam_solucao, c, *c1, *c2, peso1, peso2);
    }

    return ERRO;
}

unsigned long int funcao_para_comparacao_solucoes(int *bin_rem, int tam, int capacidade, int c1, int c2, int peso1, int peso2)
{
    int i;
    unsigned long int soma = 0, soma_pesos_bin;

    for(i = 0; i < tam; i++) {
        if (i == c1)
            soma_pesos_bin = capacidade - (bin_rem[i] + peso1 - peso2);
        else if (i == c2)
            soma_pesos_bin = capacidade - (bin_rem[i] + peso2 - peso1);
        else
            soma_pesos_bin = capacidade - bin_rem[i];

        soma += (soma_pesos_bin * soma_pesos_bin);
    }
    return soma;
}

float gera_probabilidade()
{
    int num;

    num = gera_numero_aleatorio(101);
    return ((float) num) / 100;
}

void elimina_bin(lista *bins_solucao, int *bin_rem, int tam_solucao, int indice)
{
    lista l_aux;

    l_aux = bins_solucao[indice];
    bins_solucao[indice] = bins_solucao[(tam_solucao - 1)];
    termina_lista(l_aux);

    bin_rem[indice] = bin_rem[(tam_solucao - 1)];
}

double calcula_segundos_transcorridos(clock_t t_inicio)
{
    clock_t t_fim = clock();
    return ((double) t_fim - t_inicio) / ((double) CLOCKS_PER_SEC);
}
