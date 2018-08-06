#ifndef OTIM_MIP
#define OTIM_MIP

#include "grafo.h"

/*
 * n      : nr de items no problema
 * nBins  : nr. de bins 
 * r      : capacidade restante de cada bin
 * w      : peso de cada item
 * li     : localização item : 0...nBins-1 ou -1 se não será otimizado
 * G      : grafo de conflitos
 * nli    : nova localização (bin)
 **/
void otimiza_mip( int n, int nBins, int r[], const int w[], int li[], grafo G, int nli[] );

#endif

