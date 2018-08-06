#ifndef OTIM_MIP
#define OTIM_MIP

#include "grafo.h"

// nr. de bins a otimizar
// indice bin
// capacidade restante bin
// nr de itens a otimizar
// indice item
// bin atual
// peso item
// grafo de conflitos
// bin that each iten will be moved or -1 if no better folution was found
void otimiza_mip( int n, const int w[], int li[], grafo G, int nli[] );

#endif

