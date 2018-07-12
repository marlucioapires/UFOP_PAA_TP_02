#include <stdio.h>
#include <stdlib.h>
#include "estruturas.h"

/** ASSINATURAS DE FUNÇÕES INTERNAS, CUJAS IMPLEMENTAÇÕES SE ENCONTRAM NO
  * FINAL DO ARQUIVO.
  */
int* cria_vetor_caixas_empilhadas(int**, caixa*, int, int, int*);

lista* cria_vetor_lista_de_caixas_empilhaveis(caixa*, int);

void termina_vetor_lista_de_caixas_empilhaveis(lista*, int);

int max(int*, int, int*);

/* Esta função aloca espaço em memória para uma instância do TAD problema.
 * Os parâmetros de entrada são n (o tamanho do problema, ou seja, o número
 * de caixas) e h (altura máxima da pilha de caixas).
 *
 * Retorna uma instância do TAD problema ou retorna NULL, caso haja problemas
 * na alocação dinâmica de memória ou se n menor ou igual a zero.
 *
 * Complexidade: O(1)
 */
problema cria_problema(int n, int h)
{
    problema p = NULL; // Variável que será retornada ao final da função.

    // Verifica se o valor de n é válido.
    if (n > 0) {

        // Aloca espaço em memória.
        p = (problema) malloc(sizeof(problema_empilhamento));

        // Verifica se a alocação foi bem sucedida.
        if (p) {
            p->n = n; // Tamanho do problema.
            p->h = h; // Tamanho máximo da pilha.
            p->i_caixas = 0; // Estas variáveis controlam quantas caixas e quantos
            p->i_v = 0;      // valores foram efetivamente adicionados ao problema.

            // Aloca espaço em memória para o array de caixas.
            p->caixas = (caixa*) malloc(sizeof(caixa) * n);

            // Verifica se a alocação foi bem sucedida.
            if (!p->caixas) {
                free(p); // Libera a memória alocada.
                return NULL; // Retorna NULL, devido a não ter conseguido alocar memória.

            } else {
                // Aloca memória para o array de valores (v) das caixas.
                p->v = (int*) malloc(sizeof(int) * n);

                // Verifica se a alocação foi bem sucedida.
                if (!p->v) {

                    free(p->caixas); // Libera a memória do array de caixas.
                    free(p); // Libera a memória do problema.
                    p = NULL; // O problema recebe NULL.

                }

            }
        }
    }

    return p; // Retorna uma instância do TAD problema ou NULL.
} // fim da função cria_problema


/* Esta função libera o espaço alocado para uma instância do TAD problema.
 * O parâmetro de entrada é uma variável do tipo problema, cujo espaço em
 * memória será liberado. Nenhum valor é retornado pela função.
 *
 * Complexidade: O(1)
 */
void termina_problema(problema p)
{
    if (p) {
        free(p->caixas); // Desaloca o vetor caixas.
        free(p->v); // Desaloca o vetor v.
        free(p); // Desaloca o espaço utilizado pela variável do tipo problema.
    }
} // fim da função termina_problema


/* Adiciona uma caixa à instância do TAD problema. Os parâmetros de entrada são:
 *
 *      - a variável do tipo problema;
 *      - a caixa a ser adicionada a este problema.
 *
 * Retorna 1 (um), indicando sucesso na operação, ou retorna 0 (zero), caso não
 * haja mais espaço para inserções.
 *
 * Complexidade: O(1)
 */
int add_caixa(problema p, caixa c)
{
    // A variável i_caixas controla quantas caixas de fato foram adicionadas e
    // n é o tamanho (número de caixas) do problema.
    if (p->i_caixas < p->n) {
        p->caixas[p->i_caixas++] = c;   // Adiciona a caixa (c) ao vetor de caixas (p->caixas)
        // e incrementa i_caixas.
        return 1; // Sucesso na inserção.
    }

    return 0; // Falha na inserção.
} // fim da função add_caixa


/* Adiciona um valor à instância do TAD problema. Os parâmetros de entrada são:
 *
 *      - a variável do tipo problema;
 *      - o valor a ser adicionado a este problema.
 *
 * Retorna 1 (um), indicando sucesso na operação, ou retorna 0 (zero), caso não
 * haja mais espaço para inserções.
 *
 * Complexidade: O(1)
 */
int add_valor(problema p, int v)
{
    // A variável i_v controla quantas valores de fato foram adicionados e
    // n é o tamanho (número de caixas) do problema.
    if (p->i_v < p->n) {
        p->v[p->i_v++] = v; // Adiciona o valor (v) ao vetor de valores (p->v) e incrementa i_v.
        return 1; // Sucesso na inserção.
    }

    return 0; // Falha na inserção.
} // fim da função add_valor


/* Encontra a solução ótima para o problema de empilhamento de caixas, utilizando programação
 * dinâmica e uma estratégia de resolução iterativa (bottom up). O parâmetro de entrada p
 * contém todas as informações necessárias à resolução do problema: a altura máxima da pilha
 * (h), o conjunto n de caixas (com as respectivas dimensões) e os valores associados a elas.
 * O parâmetro tam, passado por referência, receberá o tamanho do vetor criado nesta função.
 *
 * Retorna o valor da solução ótima encontrada, além de retornar um vetor (e seu tamanho) com
 * os índices das caixas utilizadas no empilhamento de altura máxima h. Em caso de falha na
 * alocação de recursos computacionais, a função retorna o valor -1, NULL no lugar do vetor
 * com os índices das caixas e o tamanho 0 (zero).
 *
 * Obs.: A função cria e retorna um vetor alocado dinamicamente. É responsabilidade do utilizador
 * desta função desalocá-lo posteriormente.
 *
 * Complexidade: O(n²h)
 */
int empilhamento_bottom_up(problema p, int** vetor_de_caixas_empilhadas, int* tam)
{
    // As matrizes a seguir são utilizadas para o cálculo da solução ótima e para o cálculo
    // da sequência de caixas utilizadas no empilhamento.
    int **matriz_emp, **matriz_indices;

    int k, i, j, h_aux, solucao_otima, pos; // Variáveis auxiliares.

    // Vetor, de listas encadeadas simples, utilizado para armazenar, para cada caixa, uma
    // relação das demais caixas que são empilháveis sobre ela (empilhamento estável).
    lista *vetor_lista_de_caixas_empilhaveis;

    tipo_nodo *no; // Variável auxiliar utilizada para percorrer a lista de caixas empilháveis.

    // Verifica se a variável passada como parâmetro representa memória alocada.
    if (*vetor_de_caixas_empilhadas)
        free(*vetor_de_caixas_empilhadas); // Libera a memória alocada.

    *vetor_de_caixas_empilhadas = NULL; // Inicialização das variáveis passadas por
    *tam = 0;                           // referência e que retornarão valores também.

    matriz_emp = (int**) malloc(sizeof(int*) * (p->h + 1)); // Aloca as linhas da matriz.
    if (!matriz_emp) // Verifica se a alocação foi bem sucedida.
        return -1; // Falha no cálculo da solução ótima, devido à ausência de recursos computacionais.

    // Aloca o restante da matriz, sem fragmentação.
    matriz_emp[0] = (int*) malloc(sizeof(int) * (p->h + 1) * (p->n));
    if (!matriz_emp[0]) { // Verifica se a alocação foi bem sucedida.
        free(matriz_emp);       // Libera os recursos alocados anteriormente.
        return -1; // Falha no cálculo da solução ótima, devido à ausência de recursos computacionais.
    }

    matriz_indices = (int**) malloc(sizeof(int*) * (p->h + 1)); // Aloca as linhas da matriz.
    if (!matriz_indices) { // Verifica se a alocação foi bem sucedida.
        free(matriz_emp[0]);    // Libera os recursos
        free(matriz_emp);       // alocados anteriormente.
        return -1; // Falha no cálculo da solução ótima, devido à ausência de recursos computacionais.
    }

    // Aloca o restante da matriz, sem fragmentação.
    matriz_indices[0] = (int*) malloc(sizeof(int) * (p->h + 1) * p->n);
    if (!matriz_indices[0]) { // Verifica se a alocação foi bem sucedida.
        free(matriz_emp[0]);    // Libera os recursos
        free(matriz_emp);       // alocados
        free(matriz_indices);   // anteriormente.
        return -1; // Falha no cálculo da solução ótima, devido à ausência de recursos computacionais.
    }

    // Cria o vetor com as listas de caixas empilháveis.
    vetor_lista_de_caixas_empilhaveis = cria_vetor_lista_de_caixas_empilhaveis(p->caixas, p->n);
    if (!vetor_lista_de_caixas_empilhaveis) { // Verifica se a alocação foi bem sucedida.
        free(matriz_emp[0]);     // Libera os
        free(matriz_emp);        // recursos
        free(matriz_indices[0]); // alocados
        free(matriz_indices);    // anteriormente.
        return -1; // Falha no cálculo da solução ótima, devido à ausência de recursos computacionais.
    }

    // Inicialização de valores.
    for(i = 0; i < p->n; i++) {
        matriz_emp[0][i] = 0; // Inicializa a primeira linha da matriz com 0 (zero).
        matriz_indices[0][i] = -1;  // Inicializa a primeira linha da matriz que guarda
        // a sequência de caixas utilizadas no empilhamento
        // com o valor -1. Este valor indica que não houve
        // empilhamento de caixas naquela posição.
    }

    for(k = 1; k <= p->h; k++) { // Percorre todos os valores de 1 até h (altura máxima da pilha).

        matriz_emp[k] = matriz_emp[(k - 1)] + p->n;         // Corrige a referência de ponteiros da alocação
        matriz_indices[k] = matriz_indices[(k - 1)] + p->n; // dinâmica de matriz sem fragmentação.

        for(i = 0; i < p->n; i++) { // Percorre todas as n caixas.

            matriz_emp[k][i] = 0;       // Inicializa a posição da matriz de soluções ótimas com 0 (zero).
            matriz_indices[k][i] = -1;  // Inicializa a posição da matriz da seq. de empilhamento com -1.

            h_aux = k - p->caixas[i].a; // Subtrai a altura da caixa da altura k da iteração.

            if (h_aux >= 0) { // Se o valor anterior for não-negativo, a caixa cabe na pilha.

                // A seguir, percorre-se a lista de caixas que são empilháveis a esta caixa
                // e verifica-se qual o empilhamento, de uma solução de subproblema menor,
                // mais o valor desta caixa apresenta o maior valor.
                no = vetor_lista_de_caixas_empilhaveis[i]->first;

                // As linhas comentadas abaixo são a forma de se percorrer a lista encadeada utilizando
                // operação do TAD lista (própria para este fim).
                //no = NULL;
                //j = get_next_chave(vetor_lista_de_caixas_empilhaveis[i], &no);

                while(no) {
                    j = no->item.chave; // Cada nó da lista contém o índice de uma caixa que é empilhável.

                    // Através da estrutura condicional a seguir, escolhe-se o maior valor dentre todos
                    // os empilhamentos possíveis. Ao final, a posição da matriz armazenará a solução
                    // ótima para o conjunto (h, l, p), onde h é representado pelo índice das linhas da
                    // matriz de soluções ótimas (matriz_emp) e as dimensões l e p estão associadas ao
                    // índice das colunas, já que ele está associado a uma caixa com dimensões (l e p).
                    //
                    // Ao mesmo tempo em que se calculam as soluções ótimas, a matriz com as sequências de
                    // empilhamentos (matriz_indices) também é gerada.
                    if ((p->v[i] + matriz_emp[h_aux][j]) > matriz_emp[k][i]) {
                        matriz_emp[k][i] = p->v[i] + matriz_emp[h_aux][j];
                        if (matriz_emp[h_aux][j] != 0) // O valor da célula (-1) só é substituído
                            matriz_indices[k][i] = j;  // se a caixa for empilhada com outra(s).
                    }

                    no = no->next; // Altera a referência do ponteiro para o próximo nó da lista.

                    // A linha comentada abaixo é a forma de se percorrer a lista encadeada utilizando
                    // operação do TAD lista (própria para este fim).
                    //j = get_next_chave(vetor_lista_de_caixas_empilhaveis[i], &no);
                }
            }
        }
    }

    // Libera a memória alocada para o vetor com as listas de caixas empilháveis.
    termina_vetor_lista_de_caixas_empilhaveis(vetor_lista_de_caixas_empilhaveis, p->n);

    // A solução ótima, para uma altura máxima h, é dada pelo valor máximo de todas
    // as soluções (h, l, p), onde h é a última linha da matriz.
    solucao_otima = max(matriz_emp[p->h], p->n, &pos);

    free(matriz_emp[0]);    // Libera a memória alocada para a matriz
    free(matriz_emp);       // de soluções ótimas.

    if (solucao_otima > 0) { // Se a variável é maior do que 0 (zero), uma solução ótima foi encontrada.
        // Gera um vetor com os índices das caixas utilizadas no empilhamento de altura máxima h.
        (*vetor_de_caixas_empilhadas) = cria_vetor_caixas_empilhadas(matriz_indices, p->caixas, p->h, pos, tam);

        if (!(*vetor_de_caixas_empilhadas)) // Verifica se a alocação foi bem sucedida.
            solucao_otima = -1; // Falha, devido à ausência de recursos computacionais.
    }

    free(matriz_indices[0]);    // Libera a memória alocada para a matriz
    free(matriz_indices);       // que guarda a sequência de empilhamento.

    return solucao_otima; // Retorna a solução ótima ou -1, em caso de falha na alocação de memória.
} // fim da função empilhamento_bottom_up


/* Função recursiva que encontra uma solução ótima para o problema de empilhamento associado
 * a uma altura máxima de pilha, largura e profundidade específicas (h, l, p). O parâmetro de
 * entrada h é a altura máxima da pilha para a qual se deseja calcular a solução. O indice_caixa,
 * através da caixa que referencia, representa as dimensões largura e profundidade (l e p) para
 * as quais esta solução é ótima.
 *
 * Os demais parâmetros de entrada são:
 *
 *      - o tamanho do problema n, ou seja, o número de caixas;
 *      - o vetor de caixas;
 *      - o vetor de valores associados às caixas;
 *      - a matriz (h x n) que será usada para armazenar as diversas soluções ótimas, que é um artifício
 *           da programação dinâmica para evitar cálculos já realizados. Na primeira chamada a esta
 *           função recursiva, a matriz deve ter sua primeira linha inicilizada com zeros e as demais
 *           células com o valor -1 (o valor menos um indica que aquela posição da matriz ainda não
 *           foi calculada);
 *      - a matriz (h x n) que será utilizada para armazenar os índices das caixas que compõem as sequências
 *           de empilhamentos. Na primeira chamada a esta função recursiva, a matriz deve ter sua primeira
 *           linha inicializada com o valor -1;
 *      - um vetor com as listas de todas as caixas que são empilháveis.
 *
 * Retorna o valor da solução ótima, para uma pilha de altura máxima h, associada a uma largura e profundidade
 * específicas da caixa de índice indice_caixa.
 *
 * Complexidade: O(nh)
 */
int empilhamento_recursiva(int h, int indice_caixa, int n, caixa* caixas, int* valores,
                           int** matriz_emp, int** matriz_indices, lista* vetor_lista_de_caixas_empilhaveis)
{
    int i, h_aux, emp_aux; // Variáveis auxiliares.
    tipo_nodo *no; // Variável do tipo tipo_nodo utilizada para percorrer a lista de caixas empilháveis.

    if (matriz_emp[h][indice_caixa] != -1)  // Se o valor procurado já foi calculado, ou seja, já se
        return matriz_emp[h][indice_caixa]; // encontra na matriz de soluções ótimas, basta retorná-lo.
    else {

        matriz_emp[h][indice_caixa] = 0;       // Inicializa a posição da matriz de soluções ótimas com 0 (zero).
        matriz_indices[h][indice_caixa] = -1;  // Inicializa a posição da matriz da seq. de empilhamento com -1.

        h_aux = h - caixas[indice_caixa].a; // Subtrai a altura da caixa da altura h (altura da solução procurada).

        if (h_aux >= 0) { // Se o valor anterior for não-negativo, a caixa cabe na pilha.

            // A seguir, percorre-se a lista de caixas que são empilháveis à caixa de índice indice_caixa
            // e verifica-se qual o empilhamento, de uma solução de subproblema menor, mais o valor desta
            // caixa apresenta o maior valor.
            no = vetor_lista_de_caixas_empilhaveis[indice_caixa]->first;

            // As linhas comentadas abaixo são a forma de se percorrer a lista encadeada utilizando
            // operação do TAD lista (própria para este fim).
            //no = NULL;
            //j = get_next_chave(vetor_lista_de_caixas_empilhaveis[indice_caixa], &no);

            while(no) {
                i = no->item.chave; // Cada nó da lista contém o índice de uma caixa que é empilhável.

                // A chamada recursiva a seguir é responsável por calcular a solução ótima de um subproblema
                // menor. Ao somar o resultado com o valor da caixa de índice indice_caixa, obtém-se uma solução
                // de empilhamento, mas que não se sabe ser a ótima.
                emp_aux = empilhamento_recursiva(h_aux, i, n, caixas,
                                                 valores, matriz_emp, matriz_indices, vetor_lista_de_caixas_empilhaveis)
                          + valores[indice_caixa];

                // Através da estrutura condicional a seguir, escolhe-se o maior valor dentre todos
                // os empilhamentos possíveis. Ao final, a posição da matriz armazenará a solução
                // ótima para o conjunto (h, l, p), onde h é representado pelo índice das linhas da
                // matriz de soluções ótimas (matriz_emp) e as dimensões l e p estão associadas ao
                // índice das colunas, já que ele está associado a uma caixa com dimensões (l e p).
                //
                // Ao mesmo tempo em que se calculam as soluções ótimas, a matriz com as sequências de
                // empilhamentos (matriz_indices) também é gerada.
                if (emp_aux > matriz_emp[h][indice_caixa]) {
                    matriz_emp[h][indice_caixa] = emp_aux;
                    if (emp_aux > valores[indice_caixa])        // O valor da célula (-1) só é substituído
                        matriz_indices[h][indice_caixa] = i;    // se a caixa for empilhada com outra(s).
                }

                no = no->next; // Altera a referência do ponteiro para o próximo nó da lista.

                // A linha comentada abaixo é a forma de se percorrer a lista encadeada utilizando
                // operação do TAD lista (própria para este fim).
                //j = get_next_chave(vetor_lista_de_caixas_empilhaveis[indice_caixa], &no);
            }
        }
    }

    return matriz_emp[h][indice_caixa]; // Retorna a solução ótima, para uma altura h, l e p específicas.
} // fim da função empilhamento_recursiva


/* Encontra a solução ótima para o problema de empilhamento de caixas, utilizando programação
 * dinâmica e uma estratégia de resolução recursiva (top down). O parâmetro de entrada p
 * contém todas as informações necessárias à resolução do problema: a altura máxima da pilha
 * (h), o conjunto n de caixas (com as respectivas dimensões) e os valores associados a elas.
 * O parâmetro tam, passado por referência, receberá o tamanho do vetor criado nesta função.
 *
 * Retorna o valor da solução ótima encontrada, além de retornar um vetor (e seu tamanho) com
 * os índices das caixas utilizadas no empilhamento de altura máxima h. Em caso de falha na
 * alocação de recursos computacionais, a função retorna o valor -1.
 *
 * Obs.: A função cria e retorna um vetor alocado dinamicamente. É responsabilidade do utilizador
 * desta função desalocá-lo posteriormente.
 *
 * Complexidade: O(n²h)
 */
int empilhamento_top_down(problema p, int** vetor_de_caixas_empilhadas, int* tam)
{
    // As matrizes a seguir são utilizadas para o cálculo da solução ótima e para o cálculo
    // da sequência de caixas utilizadas no empilhamento.
    int **matriz_emp, **matriz_indices;
    int i, j, solucao_otima, pos;

    // Vetor, de listas encadeadas simples, utilizado para armazenar, para cada caixa, uma
    // relação das demais caixas que são empilháveis sobre ela (empilhamento estável).
    lista *vetor_lista_de_caixas_empilhaveis;

    // Verifica se a variável passada como parâmetro representa memória alocada.
    if (*vetor_de_caixas_empilhadas)
        free(*vetor_de_caixas_empilhadas); // Libera a memória alocada.

    *vetor_de_caixas_empilhadas = NULL; // Inicialização das variáveis passadas por
    *tam = 0;                           // referência e que retornarão valores também.

    matriz_emp = (int**) malloc(sizeof(int*) * (p->h + 1)); // Aloca as linhas da matriz.
    if (!matriz_emp) // Verifica se a alocação foi bem sucedida.
        return -1; // Falha no cálculo da solução ótima, devido à ausência de recursos computacionais.

    // Aloca o restante da matriz, sem fragmentação.
    matriz_emp[0] = (int*) malloc(sizeof(int) * (p->h + 1) * (p->n));
    if (!matriz_emp[0]) { // Verifica se a alocação foi bem sucedida.
        free(matriz_emp); // Libera os recursos alocados anteriormente.
        return -1; // Falha no cálculo da solução ótima, devido à ausência de recursos computacionais.
    }

    matriz_indices = (int**) malloc(sizeof(int*) * (p->h + 1)); // Aloca as linhas da matriz.
    if (!matriz_indices) { // Verifica se a alocação foi bem sucedida.
        free(matriz_emp); // Libera os recursos alocados anteriormente.
        return -1; // Falha no cálculo da solução ótima, devido à ausência de recursos computacionais.
    }

    // Aloca o restante da matriz, sem fragmentação.
    matriz_indices[0] = (int*) malloc(sizeof(int) * (p->h + 1) * p->n);
    if (!matriz_indices[0]) { // Verifica se a alocação foi bem sucedida.
        free(matriz_emp[0]);    // Libera os recursos
        free(matriz_emp);       // alocados
        free(matriz_indices);   // anteriormente.
        return -1; // Falha no cálculo da solução ótima, devido à ausência de recursos computacionais.
    }

    // Cria o vetor com as listas de caixas empilháveis.
    vetor_lista_de_caixas_empilhaveis = cria_vetor_lista_de_caixas_empilhaveis(p->caixas, p->n);
    if (!vetor_lista_de_caixas_empilhaveis) { // Verifica se a alocação foi bem sucedida.
        free(matriz_emp[0]);     // Libera os
        free(matriz_emp);        // recursos
        free(matriz_indices[0]); // alocados
        free(matriz_indices);    // anteriormente.
        return -1; // Falha no cálculo da solução ótima, devido à ausência de recursos computacionais.
    }

    // Inicialização de valores.
    for(i = 0; i < p->n; i++) {
        matriz_emp[0][i] = 0; // Inicializa a primeira linha da matriz com 0 (zero).
        matriz_indices[0][i] = -1;  // Inicializa a primeira linha da matriz que guarda
        // a sequência de caixas utilizadas no empilhamento
        // com o valor -1. Este valor indica que não houve
        // empilhamento de caixas naquela posição.
    }

    for(i = 1; i <= p->h; i++) { // Percorre todos os valores de 1 até h (altura máxima da pilha).

        matriz_emp[i] = matriz_emp[(i - 1)] + p->n;         // Corrige a referência de ponteiros da alocação
        matriz_indices[i] = matriz_indices[(i - 1)] + p->n; // dinâmica de matriz sem fragmentação.

        // A seguir todas as células da linha, de ambas as matrizes, recebem o valor -1,
        // que indica para a função recursiva que uma determinada posição da matriz de
        // soluções ótimas ainda não foi calculada.
        for(j = 0; j < p->n; j++) {
            matriz_emp[i][j] = -1;
            matriz_indices[i][j] = -1;
        }
    }

    // Percorrem-se as n caixas, calculando, através da função recursiva, qual é a solução ótima
    // para a altura h. A solução ótima, para uma pilha de altura máxima h, será o máximo de todos
    // esses n cálculos.
    for(i = 0; i < p->n; i++) {
        // A função empilhamento_recursiva calcula a solução ótima para uma pilha de altura máxima
        // h e uma largura e profundidade (l e p) que estão associadas a uma caixa de índice i.
        empilhamento_recursiva(p->h, i, p->n, p->caixas, p->v, matriz_emp, matriz_indices, vetor_lista_de_caixas_empilhaveis);
    }

    // A liberação do vetor de listas é realizada através da função específica a seguir.
    termina_vetor_lista_de_caixas_empilhaveis(vetor_lista_de_caixas_empilhaveis, p->n);

    // A solução ótima será o máximo de todos os valores armazenados na linha de índice h
    // da matriz de soluções ótimas.
    solucao_otima = max(matriz_emp[p->h], p->n, &pos);

    free(matriz_emp[0]);    // Libera a memória alocada para a matriz
    free(matriz_emp);       // de soluções ótimas.

    if (solucao_otima > 0) { // Se a variável é maior do que 0 (zero), uma solução ótima foi encontrada.
        // Gera um vetor com os índices das caixas utilizadas no empilhamento de altura máxima h.
        (*vetor_de_caixas_empilhadas) = cria_vetor_caixas_empilhadas(matriz_indices, p->caixas, p->h, pos, tam);

        if (!(*vetor_de_caixas_empilhadas)) // Verifica se a alocação foi bem sucedida.
            solucao_otima = -1; // Falha, devido à ausência de recursos computacionais.
    }

    free(matriz_indices[0]);    // Libera a memória alocada para a matriz
    free(matriz_indices);       // que guarda a sequência de empilhamento.

    return solucao_otima; // Retorna a solução ótima ou -1, em caso de falha na alocação de memória.
} // fim da função empilhamento_top_down

/*********************************************************************************************************
 *                                  INÍCIO DAS FUNÇÕES INTERNAS
 *********************************************************************************************************/

/* Cria, a partir de uma matriz de índices, um vetor com a sequência de caixas utilizadas
 * em um empilhamento. A primeira posição do vetor contém o índice da caixa que é a base da
 * pilha e a última posição contém o índice da caixa que está no topo.
 *
 * Os parâmetros de entrada são:
 *
 *      - a matriz que guarda todas as sequências de empilhamentos;
 *      - o vetor de caixas (para obtenção das alturas das caixas);
 *      - a altura h do empilhamento;
 *      - a posição da última caixa empilhada;
 *      - uma variável, passada por referência, para armazenar o tamanho do vetor criado.
 *
 * Retorna um vetor e o seu tamanho. Caso haja falha na alocação de recursos computacionais,
 * a função retorna NULL e o tamanho do vetor como 0 (zero).
 *
 * Obs.: A função cria e retorna um vetor alocado dinamicamente. É responsabilidade do utilizador
 * desta função desalocá-lo posteriormente.
 *
 * Complexidade: O(h)
 */
int* cria_vetor_caixas_empilhadas(int** matriz_indices, caixa* caixas, int h, int pos_ultima_caixa_empilhada, int* tam)
{
    int *vetor = NULL, i, aux; // Vetor que será retornado pela função e variáveis auxiliares.
    lista l; // Variável auxiliar usada para criar uma lista encadeada.
    tipo_elemento e; // Variável auxiliar usada para inserção de elementos em lista encadeada.
    tipo_nodo *no; // Variável auxiliar usada para percorrer os nós de uma lista encadeada.

    *tam = 0; // O tamanho do vetor que será criado é inicializado com 0 (zero).

    // Cria-se uma lista encadeada simples para armazenar a sequência de caixas empilhadas.
    // A lista é necessária porque não se sabe o número de caixas utilizadas.
    l = cria_lista();

    if (l) { // Verifica se a lista foi alocada com sucesso.

        e.chave = pos_ultima_caixa_empilhada;   // Insere, na lista, o índice da última
        insere_apos(l, e);                      // caixa empilhada (a base da pilha).

        // O valor -1, na matriz de sequência de empilhamentos, indica que, naquela posição,
        // não houve empilhamento de uma caixa sobre outra(s).
        while (matriz_indices[h][pos_ultima_caixa_empilhada] != -1) {
            aux = matriz_indices[h][pos_ultima_caixa_empilhada];

            // Desconta, da altura h, a altura da caixa que já foi inserida na lista.
            h = h - caixas[pos_ultima_caixa_empilhada].a;

            // A seguir, pega-se o índice da caixa que está sobre a caixa que acabou de ser inserida na lista.
            pos_ultima_caixa_empilhada = aux; // Pega o valor da matriz, antes da alteração do valor de h.

            e.chave = pos_ultima_caixa_empilhada;   // Insere, no fim da lista,
            insere_apos(l, e);                      // esta caixa.
        }

        // Aloca, dinamicamente, um vetor do tamanho da lista encadeada.
        vetor = (int*) malloc(sizeof(int) * tamanho_lista(l));
        *tam = tamanho_lista(l); // O tamanho do vetor é o tamanho da lista.

        // A seguir, percorrem-se todos os nós da lista e copiam-se os índices das
        // caixas para o vetor, na mesma ordem em que eles aparecem na lista.
        no = l->first;
        i = 0;
        while(no) {
            vetor[i++] = no->item.chave; // O índice da caixa é copiado da lista para o vetor.
            no = no->next;
        }

        termina_lista(l); // Libera a memória alocada para a lista.
    }

    return vetor; // Retorna o vetor criado ou NULL.
} // fim da função cria_vetor_caixas_empilhadas


/* Cria, a partir do vetor de caixas (de tamanho n), um vetor com as listas de todas as caixas
 * que são empilháveis sobre cada caixa. A primeira posição do vetor contém uma lista de todas
 * as caixas que são empilháveis sobre a caixa de número 1 (um), a segunda posição contém uma
 * lista de todas as caixas que são empilháveis sobre a caixa de número 2 e assim por diante.
 * Uma caixa é empilhável sobre outra se ela forma uma pilha estável, onde a largura é menor ou
 * igual à da caixa de baixo e, também, a profundidade é menor ou igual à da caixa de baixo.
 *
 * Os parâmetros de entrada são:
 *
 *      - o vetor de caixas;
 *      - o tamanho n desse vetor.
 *
 * Retorna um vetor com todas as listas de caixas empilháveis. Caso haja falha na alocação de
 * recursos computacionais, a função retorna NULL.
 *
 * Obs.: A função cria e retorna um vetor alocado dinamicamente e cada posição desse vetor também
 * contém uma lista alocada dinamicamente. É responsabilidade do utilizador desta função desalocar,
 * posteriormente, cada uma das listas de cada posição do vetor e depois desalocar o vetor. A função
 * termina_vetor_lista_de_caixas_empilhaveis automatiza este processo.
 *
 * Complexidade: O(n²)
 */
lista* cria_vetor_lista_de_caixas_empilhaveis(caixa* caixas, int n)
{
    lista *vetor_adj = NULL; // Vetor que será retornado pela função.
    tipo_elemento e; // Variável auxiliar usada para inserção de elementos em lista encadeada.
    lista l; // Variável auxiliar usada para criação das listas que comporão o vetor de listas.
    int i, j; // Variáveis usadas no controle de iterações das estruturas de repetição.

    vetor_adj = (lista*) malloc(sizeof(lista) * n); // Aloca o vetor, de tamanho n.

    if (vetor_adj) { // Verifica se a alocação foi bem sucedida.

        for(i = 0; i < n; i++) { // Itera sobre o número de caixas (n).

            l = cria_lista(); // Para cada caixa, cria-se uma lista encadeada.

            if (!l) { // Verifica se a alocação foi realizada.
                // Em caso de falha, libera os recursos que já tenham sido alocados.
                termina_vetor_lista_de_caixas_empilhaveis(vetor_adj, i);
                return NULL; // Com a falha na alocação de recursos, retorna NULL.
            }

            vetor_adj[i] = l; // Atribui a lista criada a uma posição do vetor.

            // No laço a seguir, percorrem-se todas as n caixas, verificando-se quais
            // são empilháveis sobre a caixa de índice i e inserem-se seus índices (j)
            // na lista da posição i do vetor de listas de caixas empilháveis.
            for(j = 0; j < n; j++) { // Itera sobre o número de caixas (n).
                // Verifica se a caixa de índice j é empilhável sobre a caixa de índice i.
                if ((caixas[i].l >= caixas[j].l) && (caixas[i].p >= caixas[j].p)) {
                    e.chave = j;                    // Em caso positivo, insere o índice da
                    insere_apos(vetor_adj[i], e);   // caixa j na lista da caixa i.
                }
            }
        }
    }

    return vetor_adj; // Retorna o vetor de listas ou NULL.
} // fim da função cria_vetor_lista_de_caixas_empilhaveis


/* Esta função libera o espaço alocado para um vetor de listas encadeadas.
 * Os parâmetros de entrada são o vetor de listas encadeadas e seu tamanho
 * n. Nenhum valor é retornado pela função.
 *
 * Complexidade: O(n²)
 */
void termina_vetor_lista_de_caixas_empilhaveis(lista* vetor, int n)
{
    int i;

    // Percorrem-se todas as posições do vetor e liberam-se os espaços alocados
    // para cada uma das listas que o compõem.
    for(i = 0; i < n; i++) {
        termina_lista(vetor[i]); // Desaloca a lista da posição i do vetor.
    }
    free(vetor); // Por fim, libera-se a memória alocada para o vetor.
} // fim da função termina_vetor_lista_de_caixas_empilhaveis

/* Calcula o valor máximo de um vetor de inteiros.
 *
 * Parâmetros de entrada:
 *
 *      - o vetor de inteiros a ser pesquisado;
 *      - o tamanho (n) do vetor;
 *      - um parâmetro passado por referência, para armazenar a posição do
 *        vetor que contém o valor máximo.
 *
 * Retorna o valor máximo e em qual posição do vetor esse valor foi encontrado. Se
 * o vetor passado com parâmetro for NULL, retorna 0 (zero) e a posição como -1.
 *
 * Complexidade: O(n)
 */
int max(int *v, int n, int* pos)
{
    int i, maior;

    // Verifica se o vetor não é NULL.
    if (!v) {
        *pos = -1; // A posição retornada será -1.
        return 0; // Retorna 0 (zero).
    }

    maior = v[0]; // Inicializa a variável maior com a primeira posição do vetor.
    *pos = 0; // pos começa com o índice 0 (zero).

    // Itera sobre o vetor para checar se há alguma posição com valor maior do que
    // a primeira posição.
    for (i = 1; i < n; i++)
        // Caso seja maior, atualizam-se a variáveis maior e pos.
        if (v[i] > maior) {
            maior = v[i];
            *pos = i;
        }

    return maior;
} // fim da função max
