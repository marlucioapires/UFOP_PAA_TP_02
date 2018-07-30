/*
* Universidade Federal de Ouro Preto
* Departamento de Computação
* Curso de Mestrado Acadêmico em Ciência da Computação do Programa de Pós-Graduação em Ciência da Computação (PPGCC)
* Segundo Trabalho Prático da Disciplina PCC104 - Projeto e Análise de Algoritmos
* Prof.: Dr. Haroldo Gambini Santos
*
* Autor: Marlúcio Alves Pires - matrícula 2018.10391.
* E-mail: marlucioapires@gmail.com
*
* Data da última modificação: 10 de julho de 2018.
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "algoritmos_gulosos.h"
#include "funcoes.h"
#include "grafo.h"
#include "metaheuristicas.h"

int main(int argc, char* argv[])
{
    // Inicia a contagem do tempo.
    clock_t inicio = clock(), fim; // Variáveis para medição do tempo de execução.
    double segundos; // Variável auxiliar.

    float alpha = -1.0, epsilon = -1.0, temperatura = -1.0;
    long int max_seconds;
    int sim_ann = 0, ordem_dec = 0, metodo = 1;
    char arquivo_entrada[1024], arquivo_saida[1024];

    int *vetor_sobras_bins;
    lista* conteineres_com_solucao = NULL;
    int capacidade, *vetor_pesos, j, resultado = 0;
    grafo g = NULL;

    if (argc < 4) {
        printf("\nParametros invalidos!");
        printf("\nSintaxe:");
        printf("\n\tprograma arquivoInstancia arquivoSolucao maxSeconds [parametros de configuracao do metodo]\n");
    } else {
        if (!processa_parametros_entrada(argc, argv, arquivo_entrada, arquivo_saida, &alpha,
                                         &epsilon, &temperatura, &max_seconds, &sim_ann, &ordem_dec, &metodo)) {
            printf("\nErro na passagem de parametros! Execucao interrompida.\n");
            return 0;
        }

        g = processa_arquivo_entrada(arquivo_entrada, &capacidade, &vetor_pesos);

        if (!g) {
            return 0;
        }

        if (ordem_dec)
            ordena(g, vetor_pesos);

        switch (metodo) {
        case 2:
            resultado = bestFit_conflitante(vetor_pesos, capacidade, g, &conteineres_com_solucao, &vetor_sobras_bins);
            break;
        case 3:
            resultado = worstFit_conflitante(vetor_pesos, capacidade, g, &conteineres_com_solucao, &vetor_sobras_bins);
            break;
        default:
            resultado = firstFit_conflitante(vetor_pesos, capacidade, g, &conteineres_com_solucao, &vetor_sobras_bins);
            break;
        }

        printf("\nSOLUCAO INICIAL: %d", resultado);

        if (sim_ann) {
            resultado = simulated_annealing(vetor_pesos, capacidade, g, conteineres_com_solucao,
                                            vetor_sobras_bins, resultado, alpha, temperatura,
                                            epsilon, inicio, max_seconds);

            gera_arquivo_saida(arquivo_saida, conteineres_com_solucao, resultado);

            printf("\nRESULTADO COM SIMULATED ANNEALING: %d", resultado);
        }

        gera_arquivo_saida(arquivo_saida, conteineres_com_solucao, resultado);

        termina_grafo(g);
        free(vetor_pesos);
        free(vetor_sobras_bins);
        for (j = 0; j < resultado; j++)
            termina_lista(conteineres_com_solucao[j]);
        free(conteineres_com_solucao);
    }

    fim = clock(); // Termina a contagem de tempo de execução do programa.
    segundos = ((double) fim - inicio) / ((double) CLOCKS_PER_SEC); // Converte o tempo para segundos.
    printf("\nTEMPO DE EXECUCAO: %.3f\n", segundos); // Exibe o tempo de execução do programa.

    return 0;
}
