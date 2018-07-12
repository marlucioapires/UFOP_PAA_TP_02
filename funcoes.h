#ifndef FUNCOES_H_INCLUDED
#define FUNCOES_H_INCLUDED

#include "grafo.h"

grafo processa_arquivo_entrada(const char*, int*, int**, int*);

int gera_arquivo_saida(const char*, lista*, int);

void inicia_gerador_numero_aleatorio();

int gera_numero_aleatorio(int);

int processa_parametros_entrada(int, char**, char*, char *, float *, float*, float*, long int*, int*, int*, int*);

#endif // FUNCOES_H_INCLUDED
