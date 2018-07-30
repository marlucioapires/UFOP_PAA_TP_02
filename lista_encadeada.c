#include <stdlib.h>
#include <stdio.h>
#include "lista_encadeada.h"

/* Aloca espaço em memória para uma instância do TAD lista. Não há parâmetros
 * de entrada.
 *
 * Retorna uma instância do TAD lista ou retorna NULL, caso haja problemas
 * na alocação dinâmica de memória.
 *
 * Complexidade: O(1)
 */
lista cria_lista()
{
    lista l = NULL; // Inicializa a variável.

    l = (lista) malloc(sizeof(tipo_lista)); // Aloca espaço em memória.

    if(l != NULL) { // Verifica se a alocação foi bem sucedida.
        l->first = NULL; // Inicializa os ponteiros que referenciam
        l->last = NULL;  // o primeiro e o último nós da lista com NULL.
        l->tamanho = 0;  // O tamanho da lista recém criada é 0 (zero).
    }

    return l; // Retorna a lista criada ou NULL.
} // fim da função cria_lista


/* Libera o espaço alocado para uma instância do TAD lista. O parâmetro de
 * entrada é uma variável do tipo lista, cujo espaço em memória será liberado.
 * Nenhum valor é retornado pela função.
 *
 * Complexidade: O(1)
 */
void termina_lista(lista l)
{
    tipo_nodo *p; // Variável auxiliar tipo_nodo usada para iterar sobre os nós da lista.

    // A estrutura de repetição a seguir, itera sobre toda a lista, a partir do primeiro nó.
    // Ao desalocar a memória utilizada pelo primeiro nó, a lista diminui sucessivamente, até
    // que seu primeiro nó também seja NULL.
    while(l->first != NULL) {
        p = l->first; // A variável p recebe a referência do primeiro nó da lista.
        l->first = l->first->next; // O primeiro nó da lista é alterado para o seu subsequente.
        free(p); // Libera o nó p (primeiro nó da lista).
    }
    free(l); // Desaloca a memória restante.
} // fim da função termina_lista


/* Retorna o tamanho de uma instância do TAD lista. O parâmetro de entrada é
 * uma variável do tipo lista.
 *
 * Complexidade: O(1)
 */
int tamanho_lista(lista l)
{
    return l->tamanho;
} // fim da função tamanho_lista


/* Insere um elemento, como um novo nó, ao final da lista. O parâmetro de entrada
 * é a lista em que a inserção será realizada e o elemento a ser inserido.
 *
 * Retorna 1 (um) em caso de sucesso ou 0 (zero), caso haja falha na alocação de
 * memória.
 *
 * Complexidade: O(1)
 */
int insere_apos(lista l, tipo_elemento e)
{
    tipo_nodo *p; // Variável auxiliar usada para alocar memória.

    p = (tipo_nodo*) malloc(sizeof(tipo_nodo)); // Aloca a memória necessária para o nó que será inserido.

    if (!p) // Verifica se alocação falhou.
        return 0; // Retorna 0 (zero), devido a problemas de alocação de memória.

    p->item = e; // O elemento passado como parâmetro é copiado para o nó.
    p->next = NULL; // Como o nó será o último da lista, a sua referência para o próximo é NULL.

    if (l->first == NULL) // Verifica se a lista está vazia.
        l->first = p; // Se está vazia, o nó criado será o primeiro.
    else
        l->last->next = p; // Se a lista não está vazia, o nó criado será o subsequente ao último nó.

    l->last = p; // O nó criado passa a ser o último nó da lista.
    l->tamanho++; // Incrementa o tamanho da lista.

    return 1; // Retorna 1 (um) indicando sucesso na inserção.
} // fim da função insere_apos


/* Insere um elemento, como um novo nó, no início da lista. O parâmetro de entrada
 * é a lista em que a inserção será realizada e o elemento a ser inserido.
 *
 * Retorna 1 (um) em caso de sucesso ou 0 (zero), caso haja falha na alocação de
 * memória.
 *
 * Complexidade: O(1)
 */
int insere_inicio(lista l, tipo_elemento e)
{
    tipo_nodo *p; // Variável auxiliar usada para alocar memória.

    p = (tipo_nodo*) malloc(sizeof(tipo_nodo)); // Aloca a memória necessária para o nó que será inserido.

    if (!p) // Verifica se alocação falhou.
        return 0; // Retorna 0 (zero), devido a problemas de alocação de memória.

    p->item = e; // O elemento passado como parâmetro é copiado para o nó.
    p->next = l->first; // Como o nó será o primeiro da lista, a sua referência para o próximo
    // é o nó que está como primeiro atualmente.
    l->tamanho++; // Incrementa o tamanho da lista.
    l->first = p; // O nó criado passa a ser o primeiro nó da lista.

    if (l->tamanho == 1) // Se a lista tem tamanho um, isso significa que o nó inserido
        l->last = p;     // também será o último nó da lista.

    return 1; // Retorna 1 (um) indicando sucesso na inserção.
} // fim da função insere_inicio


/* Permite obter o valor armazenado em um nó de uma lista, com um artifício que permite manter
 * a posição do nó e assim percorrer todos os nós sequencialmente. Os parâmetros de entrada são
 * a lista que se deseja consultar e a referência para o último nó lido. Se último nó lido é NULL,
 * retorna-se o conteúdo do primeiro nó da lista. Caso contrário, retorna-se o conteúdo do nó
 * subsequente ao último nó lido.
 *
 * Retorna o conteúdo de um nó da lista e retorna também a referência para o nó, através do parâmetro
 * de entrada tipo_nodo* passado por referência.
 *
 * Complexidade: O(1)
 */
tipo_chave get_next_chave(lista l, tipo_nodo** n)
{
    if ((*n) == NULL)       // Se o parâmetro é NULL o nó lido
        (*n) = l->first;    // será o primeiro nó da lista.
    else
        (*n) = (*n)->next; // Caso contrário, o nó lido será o nó subsequente ao último lido.

    if (*n)
        return (*n)->item.chave; // Retorna o conteúdo do nó.
    else
        return 0; // Retorna 0 (zero) em caso de não haver nó (NULL).
} // fim da função get_next_chave

void imprime_lista(lista l)
{
    tipo_nodo *p = l->first; // Variável auxiliar tipo_nodo usada para iterar sobre os nós da lista.

    while(p != NULL) {
        printf("%d -> ", p->item.chave + 1);
        p = p->next;
    }
    printf("NULL");
}

int pesquisa_chave(lista l, tipo_chave ch)
{
    tipo_nodo *p;

    p = l->first;
    while(p != NULL) {
        if(p->item.chave == ch) {
            return 1;
        } else
            p = p->next;
    }

    return 0;
}

int troca_chaves(lista l1, int pos1, lista l2, int pos2)
{
    tipo_nodo *p1, *p2;
    int i, j, pos_aux;
    lista l_aux;
    tipo_elemento e;

    if (pos1 > pos2) {
        l_aux = l1;
        l1 = l2;
        l2 = l_aux;
        pos_aux = pos1;
        pos1 = pos2;
        pos2 = pos_aux;
    }

    p1 = l1->first;
    p2 = l2->first;
    for(i = 1; i < pos1; i++) {
        p1 = p1->next;
        p2 = p2->next;
    }

    for(j = i; j < pos2; j++)
        p2 = p2->next;

    e.chave = p1->item.chave;
    p1->item.chave = p2->item.chave;
    p2->item.chave = e.chave;

    return 1;
}

tipo_chave get_chave_posicao(lista l, int pos)
{
    tipo_nodo *p;
    int i;

    p = l->first;
    for (i = 1; i < pos && p; i++) {
        p = p->next;
    }

    if (p)
        return p->item.chave;
    else
        return -1;
}

int transfere_elemento(lista l1, int indice, lista l2)
{
    int i;
    tipo_nodo *p, *a;
    tipo_elemento e;

    /* Verificando se é uma posição válida */
    if (indice < 1 || indice > l1->tamanho)
        return 0;
    else {
        p = l1->first;
        for(i = 1; i < indice; i++) {
            a = p;
            p = p->next;
        }
        if(l1->first == p)
            l1->first = p->next;
        else if(l1->last == p) {
            l1->last = a;
            a->next = NULL;
        } else
            a->next = p->next;
        e = p->item;
        insere_apos(l2, e);
        free(p);
        l1->tamanho--;
        return 1;
    }
}

tipo_chave* retorna_vetor_chaves_lista(lista l, int *tamanho) {
    int tam = l->tamanho;
    tipo_chave *vetor = NULL;
    tipo_nodo *p;

    if (tam > 0) {
        vetor = (tipo_chave*) malloc(sizeof(tipo_chave) * tam);
        (*tamanho) = 0;
        if (vetor) {
            p = l->first;
            while(p) {
                vetor[(*tamanho)] = p->item.chave;
                (*tamanho)++;
                p = p->next;
            }
        }
    }

    return vetor;
}

int pesquisa_indices_vetor_na_lista(lista l, int *vetor)
{
    tipo_nodo *p;

    p = l->first;
    while(p) {
        if (vetor[p->item.chave] == 1)
            return 1;
        p = p->next;
    }

    return 0;
}

int pesquisa_indices_vetor_na_lista_exceto(lista l, int *vetor, int indice_excluido)
{
    tipo_nodo *p;

    p = l->first;
    while(p) {
        if (p->item.chave != indice_excluido) {
            if (vetor[p->item.chave] == 1)
                return 1;
        }
        p = p->next;
    }

    return 0;
}
