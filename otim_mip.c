#include "otim_mip.h"
#include "lp.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * n      : nr de items no problema
 * nBins  : nr. de bins 
 * r      : capacidade restante de cada bin
 * w      : peso de cada item
 * li     : localização item : 0...nBins-1 ou -1 se não será otimizado
 * G      : grafo de conflitos
 * nli    : nova localização (bin)
 **/
void otimiza_mip( int n, int nBins, const int r[], const int w[], int li[], grafo G, int nli[] )
{
    LinearProgram *mip = lp_create();

    char *ivBin; 
    ivBin = calloc( n, sizeof(char) );
    int *binIdx;
    binIdx =  calloc( n, sizeof(int) );



    free( binIdx );
    free( ivBin );
    lp_free(&mip);
}

