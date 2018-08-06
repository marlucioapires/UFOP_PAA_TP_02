#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "metaheuristicas.h"
#include "funcoes.h"
#include "otim_mip.h"

#define MAX_IT_SEM_M 1000
#define MAX_BINS_O 5
#define MAX_ITEMS_BIN 100


/** ASSINATURAS DE FUNÇÕES INTERNAS, CUJAS IMPLEMENTAÇÕES SE ENCONTRAM NO
  * FINAL DO ARQUIVO.
  */

void realiza_troca_bins(int*, int, lista*, int*, int*, int, int, int, int);
void escolhe_itens_permuta(int*, int*, int, int*, int*, lista*);
unsigned long int computa_proxima_solucao(int*, int*, int, int*, int*, int*, int, grafo, lista*,
        int*, int);
unsigned long int funcao_para_comparacao_solucoes(int*, int, int, int, int, int, int);
float gera_probabilidade();
void elimina_bin(lista*, int*, int, int);
double calcula_segundos_transcorridos(clock_t);

// nr. de bins a otimizar
// indice bin
// capacidade restante bin
// nr de itens a otimizar
// indice item
// bin atual
// peso item
// grafo de conflitos
/*void otimiza_mip( int nBins, int *ibin, int *r,
        int nItens, int *item, int *bin, int *w, grafo g ) {
    int aux = MAX_BINS_O, *vetor_bins_sorteados, todos = 0, i, j, k, repetido;
    int *vetor_itens_selecionados;

    if (nBins < MAX_BINS_O) {
        aux = nBins;
        todos = 1;
    }

    vetor_bins_sorteados = (int*) malloc(sizeof(int) * aux);

    if (todos) {
        for(i = 0; i < aux; i++)
            vetor_bins_sorteados[i] = i;
    } else {
        for(i = 0; i < aux; i++) {
            do{
                vetor_bins_sorteados[i] = gera_numero_aleatorio(nBins);
                repetido = 0;
                j = 0;
                while ((j < i) && (!repetido)) {
                    if (vetor_bins_sorteados[i] == vetor_bins_sorteados[j])
                        repetido = 1;
                    else
                        j++;
                }
            } while (repetido);
        }
    }

    vetor_itens_selecionados = (int*) malloc(sizeof(int) * tamanho_grafo(g));

    for(i = 0; i < aux; i++) {
        k = 0; // Controla quantos itens daquele bin já foram selecionados.
        for (j = 0; j < tamanho_grafo(g); j++) {
            if (bin[j] == vetor_bins_sorteados[i]) {
                vetor_itens_selecionados[j] = bin[j];
                k++;
                if (k == MAX_ITEMS_BIN) // Se já se selecionou o número max. de itens
                    break;              // interrompe o laço e parte para o prox. bin.
            } else {
                vetor_itens_selecionados[j] = -1;
            }
        }
    }

    free(vetor_bins_sorteados); // Libera o vetor auxiliar utilizado para sortear os bins.
    








    //free(vetor_itens_selecionados);
}*/


/* Implementação da metaheurística Simulated Annealing. Esta técnica começa sua busca a partir
 * de uma solução inicial. O procedimento principal consiste em um loop que gera aleatoriamente,
 * em cada iteração, um único vizinho s' da solução corrente s. Seja Delta a variação de valor
 * da função objetivo ao mover-se para uma solução vizinha candidata, ou seja, Delta = f(s') -
 * f(s). O método aceita o movimento e a solução vizinha passa a ser a nova solução corrente se
 * Delta < 0. Caso Delta >= 0, a solução vizinha candidata também poderá ser aceita, mas neste
 * caso, com uma probabilidade e^(-Delta/temperatura).
 *
 * Parâmetros de entrada:
 *      - weight: vetor com os pesos dos itens;
 *      - capacidade: capacidade dos contêineres (bins);
 *      - g: grafo com a representação dos conflitos existentes entre os itens;
 *      - bins_solucao: vetor com a solução inicial do problema. Cada posição deste vetor
 *              representa um contêiner (bin) e contém uma lista dos itens alocados nele;
 *      - bin_rem: vetor com a capacidade não utilizada (remanescente) dos bins;
 *      - tam_solucao: tamanho da solução inicial (número de bins utilizados);
 *      - alpha: parâmetro de configuração do Simulated Annealing que configura a taxa de
 *              decaimento da temperatura. Deve ter um valor entre ]0,1[.
 *      - temperatura: parâmetro de configuração do Simulated Annealing;
 *      - epsilon: parâmetro de configuração do Simulated Annealing que representa a temperatura
 *              mais baixa aceitável. Caso a temperatura caia abaixo do valor epsilon, a função
 *              é finalizada;
 *      - tempo_inicial: tempo decorrido entre o início da execução do programa e a chamada do
 *              método simulated_annealing;
 *      - segundos_tempo_maximo: tempo máximo em segundos, passado como parâmetro pelo usuário
 *              via chamada por linha de comando, que determina o tempo máximo de execução do
 *              programa e do método simulated_annealing.
 *
 * Retorna o valor da nova solução, ou seja, o número de contêineres (bins), após as buscas por
 * novas soluções realizadas através do Simulated Annealing.
 *
 */
int simulated_annealing(int *weight, int capacidade, grafo g, lista *bins_solucao, int *bin_rem,
                        int tam_solucao, double alpha, double temperatura, double epsilon,
                        clock_t tempo_inicial, long int segundos_tempo_maximo)
{

    int j, c1, c2, pos_i1, pos_i2, trocar_solucao; // Variáveis auxiliares.

    unsigned long int sol1, sol2; // Variáveis auxiliares usadas para armazenar os valores da
    // solução inicial s (sol1) e da solução s' (sol2).

    long int lower_bound = 0; // Variável auxiliar usada para cálculo do lower bound.

    // Variável auxiliar usada para cálculo da probabilidade de aceitação de uma solução pior.
    float prob;

    float delta; // Variável auxiliar usada para cálculo do Delta (f(s') - f(s)).

    // Validação do parâmetro alpha.
    if (alpha <= 0 || alpha >= 1) // Caso seja passado um valor inválido, como parâmetro,
        alpha = 0.999;            // atribui-se um valor default a alpha (0,999).

    // Cálculo do lower bound: soma dos pesos de todos os itens dividida pela capacidade dos
    // contêineres (bins).
    for(j = 0; j < tamanho_grafo(g); j++)
        lower_bound += weight[j];

    // Validação do parâmetro temperatura.
    if (temperatura <= 0)                        // Em caso de valor inválido, atribui-se um
        temperatura = lower_bound * lower_bound; // valor default, que é (lower bound)².

    lower_bound = ((float) lower_bound / capacidade); // Finalização do cálculo do lower bound.

    // Validação do parâmetro epsilon.
    if (epsilon <= 0)    // Em caso de valor inválido, atribui-se um
        epsilon = 0.001; // valor default a epsilon (0,001).

    inicia_gerador_numero_aleatorio(); // "Semeia" a geração de números aleatórios.

    int nrIterSemMelhora = 0;

    // Calcula a função de avaliação (função objetivo) da solução inicial.
    sol1 = funcao_para_comparacao_solucoes(bin_rem, tam_solucao, capacidade, 0, 0, 0, 0);

    // Loop do Simulated Annealing. Critérios de parada do loop:
    //      - Atingimento do tempo máximo de execução estipulado pelo usuário;
    //      - Resfriamento da temperatura abaixo do limite inferior (epsilon);
    //      - Tamanho da solução se torna igual ao lower bound (solução ótima).
    while ( (calcula_segundos_transcorridos(tempo_inicial) < segundos_tempo_maximo) &&
            (temperatura > epsilon) && (tam_solucao > lower_bound) ) {

        if ( (nrIterSemMelhora++) >= MAX_IT_SEM_M )
        {


            otimiza_mip( 0, 0, NULL, NULL, NULL, NULL );

        }


        // Gera aleatoriamente uma solução vizinha e computa a função de avaliação.
        // Apenas computa uma nova solução, sem realizar movimentações de itens entre
        // contêineres (bins).
        sol2 = computa_proxima_solucao(&c1, &c2, tam_solucao, &pos_i1, &pos_i2, weight,
                                       capacidade, g, bins_solucao, bin_rem, tam_solucao);

        if (sol2 != ERRO) { // Verifica se não retornou erro.

            delta = sol2 - sol1; // Calcula o valor de Delta (f(s') - f(s)).

            // Verifica se a solução anterior será trocada pela nova solução gerada.
            trocar_solucao = 1;
            if (delta <= 0) {
                prob = gera_probabilidade(); // Gera probabilidade entre 0 e 100%.

                // Como o valor de Delta <= 0, calcula-se a probabilidade de troca.
                // Se a probabilidade for maior que e^(-Delta/temperatura), a solução
                // anterior não será trocada pela solução gerada. Caso contrário,
                // troca-se a solução anterior pela nova solução (que é pior!).
                if (prob > exp(-delta/temperatura))
                    trocar_solucao = 0; // A solução não será trocada.
            }

            if (trocar_solucao) { // Verifica se a solução será trocada.
                sol1 = sol2; // Troca a solução.

                nrIterSemMelhora = 0;

                // Concretiza a troca de soluções, realizando a movimentação de itens entre
                // os contêineres (bins).
                realiza_troca_bins(weight, capacidade, bins_solucao, bin_rem, &tam_solucao,
                                   c1, c2, pos_i1, pos_i2);
            }
            temperatura *= alpha; // Diminui a temperatura em função de alpha.
        }
    }
    return tam_solucao; // Retorna a nova solução gerada com o Simulated Annealing.
} // fim da função simulated_annealing

/************************************************************************************************
 *                                  INÍCIO DAS FUNÇÕES INTERNAS
 ************************************************************************************************/

/* Realiza uma mudança/movimentação de itens entre contêiners (bins). Os parâmetros c1 e c2
 * são os índices dos contêineres selecionados para a troca e os parâmetros pos_i1 e pos_i2
 * indicam quais itens desses contêineres serão trocados. Se algum parâmetro de posição de
 * item for igual a -1, significa que não haverá troca de lugar entre os itens, mas sim uma
 * transferência de um item somente, de um contêiner para o outro.
 *
 * Ex.: Se pos_i1 é igual a -1, o item pos_i2 será transferido do contêiner c2 para c1.
 */
void realiza_troca_bins(int *weight, int capacidade, lista *bins_solucao, int *bin_rem,
                        int *tam_solucao, int c1, int c2, int pos_i1, int pos_i2)
{
    int item1, item2, peso1, peso2; // Variáveis auxiliares.

    if (pos_i1 == -1) { // Se pos_i1 é igual a -1, transfere o item pos_i2 de c2 para c1.

        item2 = get_chave_posicao(bins_solucao[c2], pos_i2); // Obtém o número do item 2.

        peso2 = weight[item2]; // Obtém o peso do item 2.

        // Transfere o item 2 da lista de itens do contêiner 2 (c2) para a lista de itens
        // do contêiner 1 (c1).
        transfere_elemento(bins_solucao[c2], pos_i2, bins_solucao[c1]);

        bin_rem[c1] -= peso2; // Atualiza as capacidades remanescentes dos
        bin_rem[c2] += peso2; // dois contêineres.

        // A seguir, verifica-se se o contêiner de onde saiu o item está vazio.
        if (bin_rem[c2] == capacidade) {
            // Se está vazio, elimina o bin da solução.
            elimina_bin(bins_solucao, bin_rem, (*tam_solucao), c2);
            (*tam_solucao)--; // Decrementa o número de bins (solução).
        }
    } else if (pos_i2 == -1) { // Se pos_i2 é igual a -1, transfere o item pos_i1 de c1 para c2.

        item1 = get_chave_posicao(bins_solucao[c1], pos_i1); // Obtém o número do item 1.

        peso1 = weight[item1]; // Obtém o peso do item 1.

        // Transfere o item 1 da lista de itens do contêiner 1 (c1) para a lista de itens
        // do contêiner 2 (c2).
        transfere_elemento(bins_solucao[c1], pos_i1, bins_solucao[c2]);

        bin_rem[c1] += peso1; // Atualiza as capacidades remanescentes dos
        bin_rem[c2] -= peso1; // dois contêineres.

        // A seguir, verifica-se se o contêiner de onde saiu o item está vazio.
        if (bin_rem[c1] == capacidade) {
            // Se está vazio, elimina o bin da solução.
            elimina_bin(bins_solucao, bin_rem, (*tam_solucao), c1);
            (*tam_solucao)--; // Decrementa o número de bins (solução).
        }
    } else { // Senão, troca os dois itens de lugar.

        item1 = get_chave_posicao(bins_solucao[c1], pos_i1); // Obtém o número do item 1.
        item2 = get_chave_posicao(bins_solucao[c2], pos_i2); // Obtém o número do item 2.

        peso1 = weight[item1]; // Obtém o peso do item 1.
        peso2 = weight[item2]; // Obtém o peso do item 2.

        // Troca os dois itens de lugar, ou seja, o item 1 vai para o contêiner 2 (c2) e o
        // item 2 vai para o contêiner 1 (c1).
        troca_chaves(bins_solucao[c1], pos_i1, bins_solucao[c2], pos_i2);

        bin_rem[c1] += peso1 - peso2; // Atualiza as capacidades remanescentes dos
        bin_rem[c2] += peso2 - peso1; // dois contêineres.
    }
} // fim da função realiza_troca_bins

/* Seleciona, aleatoriamente, dois contêineres e dois itens (um de cada um deles). Os
 * contêineres são representados por listas encadeadas simples, que contêm os itens
 * alocados em seu interior.
 */
void escolhe_itens_permuta(int* c1, int* c2, int max_c, int* pos_i1, int* pos_i2,
                           lista *bins_solucao)
{
    if (max_c > 1) { // max_c é o número total de contêineres.
        *c1 = gera_numero_aleatorio(max_c); // Escolhe um contêiner dentre todos.
        do {
            *c2 = gera_numero_aleatorio(max_c); // Escolhe outro contêiner dentre todos.
        } while(*c1 == *c2); // Repete a escolha até que se selecionem dois distintos.

        // Escolhe um item dentre todos os itens contidos no 1º contêiner selecionado.
        (*pos_i1) = gera_numero_aleatorio(tamanho_lista(bins_solucao[*c1])) + 1;

        // Escolhe um item dentre todos os itens contidos no 2º contêiner selecionado.
        (*pos_i2) = gera_numero_aleatorio(tamanho_lista(bins_solucao[*c2])) + 1;
    }
} // fim da função escolhe_itens_permuta

/* Escolhe, aleatoriamente, dois contêineres (bins) e dois itens (um de cada bin), verifica
 * se é possível trocar esses itens de contêiner (entre si ou somente um deles), formando
 * assim uma solução vizinha. Retorna o valor da função de avaliação (função objetivo) da
 * solução vizinha gerada ou retorna ERRO (-1) em caso de não ter encontrado uma solução
 * vizinha válida.
 */
unsigned long int computa_proxima_solucao(int* c1, int* c2, int max_c, int* pos_i1, int* pos_i2,
        int *weight, int c, grafo g, lista *bins_solucao,
        int *bin_rem, int tam_solucao)
{
    int peso1, peso2, item1, item2, escolha; // Variáveis auxiliares.

    // Gera os índices dos bins e dos itens que serão trocados.
    escolhe_itens_permuta(c1, c2, max_c, pos_i1, pos_i2, bins_solucao);

    item1 = get_chave_posicao(bins_solucao[(*c1)], *pos_i1); // Obtém o número do item 1.
    item2 = get_chave_posicao(bins_solucao[(*c2)], *pos_i2); // Obtém o número do item 2.

    peso1 = weight[item1]; // Obtém o peso do item 1.
    peso2 = weight[item2]; // Obtém o peso do item 2.

    escolha = gera_numero_aleatorio(2); // Gera um número aleatório entre 0 e 1.

    if (escolha == 0) { // Se 0, tenta transferir somente um item, de um bin para o outro.
        if (bin_rem[*c2] - peso1 >= 0) { // Verifica se o item 1 cabe no bin 2.
            // Verifica se haverá conflitos na transferência do item 1 para o bin 2.
            if (pesquisa_adjascente(g, item1, bins_solucao[*c2]) == 0) {
                *pos_i2 = -1; // O retorno -1 indica que o item 2 não será transferido. Só o item 1.

                // A seguir, retorna-se o valor da função de avaliação da solução gerada.
                return funcao_para_comparacao_solucoes(bin_rem, tam_solucao, c, *c1, *c2, peso1, 0);
            }
        } else if (bin_rem[*c1] - peso2 >= 0) { // Verifica se o item 2 cabe no bin 1.
            // Verifica se haverá conflitos na transferência do item 2 para o bin 1.
            if (pesquisa_adjascente(g, item2, bins_solucao[*c1]) == 0) {
                *pos_i1 = -1; // O retorno -1 indica que o item 1 não será transferido. Só o item 2.

                // A seguir, retorna-se o valor da função de avaliação da solução gerada.
                return funcao_para_comparacao_solucoes(bin_rem, tam_solucao, c, *c1, *c2, 0, peso2);
            }
        }
    } else // Nesse caso, tenta trocar os itens, um para o lugar do outro.
        // Verifica se os itens, quando trocados, respeitarão as capacidades dos bins.
        if ((bin_rem[*c1] + peso1 - peso2 >= 0) && (bin_rem[*c2] + peso2 - peso1 >= 0)) {
            // Verifica se haverá conflitos na troca dos dois itens.
            if ((pesquisa_adjascente_exceto(g, item1, bins_solucao[*c2], item2) == 0) &&
                    (pesquisa_adjascente_exceto(g, item2, bins_solucao[*c1], item1) == 0))
                // Retorna o valor da função de avaliação da solução gerada.
                return funcao_para_comparacao_solucoes(bin_rem, tam_solucao, c, *c1, *c2, peso1, peso2);
        }

    return ERRO; // Retorna ERRO (-1), se não encontra uma solução válida.
} // fim da função computa_proxima_solucao


/* Função para análise/comparação de soluções do problema de bin packing. A função é a soma dos
 * quadrados dos pesos preenchidos dos contêineres (bins):
 *
 * f(s) = (soma dos pesos dos itens do bin 1)² + ... + (soma dos pesos dos itens do bin n)²
 *
 * onde n é o número de contêineres (bins) utilizados na solução s.
 *
 * O valor computado pela função é retornado.
 */
unsigned long int funcao_para_comparacao_solucoes(int *bin_rem, int tam, int capacidade, int c1,
        int c2, int peso1, int peso2)
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

        soma += (soma_pesos_bin * soma_pesos_bin); // Eleva a soma dos pesos ao quadrado.
    }
    return soma; // Retorna a soma calculada.
} // fim da função funcao_para_comparacao_solucoes

/* Gera, aleatoriamente, um número real entre 0 e 1.
 */
float gera_probabilidade()
{
    int num; // Variável auxiliar.

    num = gera_numero_aleatorio(101); // Gera um número entre 0 e 100.
    return ((float) num) / 100; // Retorna um número real entre 0 e 1.
} // fim da função gera_probabilidade

/* Elimina um contêiner (bin) de um vetor de bins.
 */
void elimina_bin(lista *bins_solucao, int *bin_rem, int tam_solucao, int indice)
{
    lista l_aux; // Variável auxiliar.

    l_aux = bins_solucao[indice]; // Salva a referência da lista que representa
                                  // o bin a ser eliminado.

    // Elimina o bin do vetor de bins, substituindo-o pelo último bin do vetor.
    bins_solucao[indice] = bins_solucao[(tam_solucao - 1)];

    // Desaloca a memória utilizada pela lista que representa o bin eliminado.
    termina_lista(l_aux);

    // Atualiza o vetor que armazena as capacidades remanescentes dos bins.
    bin_rem[indice] = bin_rem[(tam_solucao - 1)];
} // fim da função elimina_bin

/* Calcula e retorna quantos segundos decorreram a partir do tempo t_inicio passado
 * como parâmetro.
 */
double calcula_segundos_transcorridos(clock_t t_inicio)
{
    clock_t t_fim = clock();
    return ((double) t_fim - t_inicio) / ((double) CLOCKS_PER_SEC);
} // fim da função calcula_segundos_transcorridos
