#include "otim_mip.h"
#include "lp.h"

/*
 * n   : nr de items no problema
 * w   : peso de cada item
 * li  : localização item : 0...nBins-1 ou -1 se não será otimizado
 * G   : grafo de conflitos
 * nli : nova localização (bin)
 **/
void otimiza_mip( int n, const int w[], int li[], grafo G, int nli[] )
{
    LinearProgram *mip = lp_create();

    lp_free(&mip);
}

