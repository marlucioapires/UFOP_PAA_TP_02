#ifndef LISTA_ENCADEADA_H_INCLUDED
#define LISTA_ENCADEADA_H_INCLUDED

typedef int tipo_chave;

typedef struct {
    tipo_chave chave;
    /* demais componentes */
} tipo_elemento;

typedef struct nodo {
    tipo_elemento item;
    struct nodo *next;
} tipo_nodo;

typedef struct {
    tipo_nodo *first, *last;
    int tamanho;
} tipo_lista;

typedef tipo_lista* lista;

lista cria_lista();

void termina_lista(lista);

int tamanho_lista(lista);

int insere_apos(lista, tipo_elemento);

int insere_inicio(lista, tipo_elemento);

tipo_chave get_next_chave(lista, tipo_nodo**);

void imprime_lista(lista);

int pesquisa_chave(lista, tipo_chave);

int troca_chaves(lista, int, lista, int);

tipo_chave get_chave_posicao(lista, int);

int transfere_elemento(lista, int, lista);

tipo_chave* retorna_vetor_chaves_lista(lista, int*);

int pesquisa_indices_vetor_na_lista(lista, int*);

int pesquisa_indices_vetor_na_lista_exceto(lista, int*, int);

int preenche_vetor_indices_lista(int*, int, lista, int);

#endif // LISTAENCADEADA_H_INCLUDED
