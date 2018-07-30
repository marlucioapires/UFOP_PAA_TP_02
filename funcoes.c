#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "funcoes.h"

#define TAM_LINHA 8192

grafo processa_arquivo_entrada(const char* nome_arquivo, int* capacidade, int**vetor_pesos)
{
    FILE *ptr_arq;
    int n, c, i, nr_item, peso_item, conflito_item;
    char linha[TAM_LINHA], *ptr_str;
    grafo g = NULL;
    int* vetor = NULL;

    ptr_arq = fopen(nome_arquivo, "r");

    if (ptr_arq != NULL) {

        if (fscanf(ptr_arq, "%d%d", &n, &c) == 2) {
            g = cria_grafo(n);

            *capacidade = c;

            if (!g) {
                fclose(ptr_arq);
                return g;
            }

            vetor = (int*) malloc(sizeof(int) * n);
            if (!vetor) {
                termina_grafo(g);
                fclose(ptr_arq);
                return NULL;
            }


            for(i = 0; i < n && !feof(ptr_arq); i++) {
                fscanf(ptr_arq, "%[\n\r]s", linha);
                fscanf(ptr_arq, "%[^\n\r]s", linha);


                ptr_str = strtok(linha, " ");
                nr_item = atoi(ptr_str);
                ptr_str = strtok(NULL, " ");
                peso_item = atoi(ptr_str);
                vetor[(nr_item - 1)] = peso_item;

                ptr_str = strtok(NULL, " ");

                while(ptr_str != NULL) {
                    conflito_item = atoi(ptr_str);
                    adiciona_aresta(g, (nr_item - 1), (conflito_item - 1));
                    ptr_str = strtok(NULL, " ");
                }
            }
            *vetor_pesos = vetor;
        }
        fclose(ptr_arq);
    }

    return g;
}

int gera_arquivo_saida(const char* nome_arquivo, lista* bins_com_solucao, int num_bins)
{
    FILE *ptr_arq; // Ponteiro para o arquivo a ser manipulado.
    int i, j, tam; // Variável auxiliar.
    int *vetor_aux;

    ptr_arq = fopen(nome_arquivo, "w");

    if (ptr_arq != NULL) {

        fprintf(ptr_arq, "%d\n", num_bins);

        for(i = 0; i < num_bins; i++) {

            fprintf(ptr_arq, "%d ", tamanho_lista(bins_com_solucao[i]));

            vetor_aux = retorna_vetor_chaves_lista(bins_com_solucao[i], &tam);

            if (!vetor_aux) {
                fclose(ptr_arq);
                return 0;
            }

            for(j = 0; j < tam; j++) {
                fprintf(ptr_arq, "%d ", vetor_aux[j] + 1);
            }

            free(vetor_aux);

            fprintf(ptr_arq, "\n");
        }

        fclose(ptr_arq); // Fecha o arquivo.

        return 1; // Sucesso na gravação do arquivo.
    }

    return 0; // Falha na gravação do arquivo.
}

void inicia_gerador_numero_aleatorio()
{
    srand(time(NULL));
}

int gera_numero_aleatorio(int a)
{
    return rand( ) % a;
}

int processa_parametros_entrada(int argc, char**argv, char *arquivo_entrada, char *arquivo_saida, float *alpha,
                                float *epsilon, float *temperatura, long int *max_seconds, int *sim_ann,
                                int *ordem_dec, int *metodo)
{
    int i;
    char* ptr_str, str_aux[60];

    if (argc < 4) // Número mínimo de parâmetros é 4 (quatro).
        return 0; // Retorna 0 (zero), devido à falha.

    strcpy(arquivo_entrada, argv[1]); // O parâmetro de índice 1 (um) é o arquivo com a instância do problema.
    strcpy(arquivo_saida, argv[2]); // O parâmetro de índice 2 (dois) é o arquivo em que será gravada a solução.

    (*max_seconds) = atol(argv[3]); // O parâmetro 3 (três) é o tempo máximo em segundos.

    for (i = 4; i < argc; i++) { // Processa os demais parâmetros passados via linha de comando.

        ptr_str = strtok(argv[i], "=");

        if (ptr_str != NULL) { // Verifica se há erro na sintaxe.

            strcpy(str_aux, ptr_str);
            ptr_str = strtok(NULL, "=");

            if (ptr_str != NULL) {

                if ((strcmp(str_aux, "a") == 0) || (strcmp(str_aux, "A") == 0)) {
                    (*alpha) = atof(ptr_str);
                } else if ((strcmp(str_aux, "e") == 0) || (strcmp(str_aux, "E") == 0)) {
                    (*epsilon) = atof(ptr_str);
                } else if ((strcmp(str_aux, "s") == 0) || (strcmp(str_aux, "S") == 0)) {
                    (*sim_ann) = atoi(ptr_str);
                } else if ((strcmp(str_aux, "d") == 0) || (strcmp(str_aux, "D") == 0)) {
                    (*ordem_dec) = atoi(ptr_str);
                } else if ((strcmp(str_aux, "m") == 0) || (strcmp(str_aux, "M") == 0)) {
                    (*metodo) = atoi(ptr_str);
                } else if ((strcmp(str_aux, "t") == 0) || (strcmp(str_aux, "T") == 0)) {
                    (*temperatura) = atof(ptr_str);
                } else {
                    printf("\nParametro \"%s\" nao existe!", str_aux);
                    return 0;
                }
            } else
                return 0;
        } else
            return 0;
    }
    return 1;
}
