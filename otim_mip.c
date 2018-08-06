#include "otim_mip.h"
#include "lp.h"
#include <stdio.h>
#include <stdlib.h>
#include "containers.h"

/*
 * n      : nr de items no problema
 * nBins  : nr. de bins 
 * r      : capacidade restante de cada bin
 * w      : peso de cada item
 * li     : localização item : 0...nBins-1 ou -1 se não será otimizado
 * G      : grafo de conflitos
 * nli    : nova localização (bin)
 **/
void otimiza_mip( int n, int c, int nBins, const int r[], const int w[], int li[], grafo G, int nli[] )
{
    LinearProgram *mip = lp_create();

    char *ivBin; 
    ivBin = calloc( n, sizeof(char) );
    int *binIdx; // binIdx[i] is which is the new index of bin i
    binIdx =  calloc( n, sizeof(int) );

    int *idxBin; // idxBin[i] returns the old index of bin i
    idxBin =  calloc( n, sizeof(int) );

    int *itemIdx; // itemIdx[i] returns the new index of item i
    itemIdx =  calloc( n, sizeof(int) );

    int *idxItem; // itemIdx[i] returns the old index of item i
    idxItem =  calloc( n, sizeof(int) );

    for ( int i=0 ; (i<n) ; ++i )
        binIdx[i] = -1;

    
    // number of discovered bins while traversing list
    int nDB = 0;
    int nDI = 0;

    for ( int i=0 ; (i<n) ; ++i )
    {
        if (li[i] != -1)
        {
            if (ivBin[li[i]]==0)
            {
                ivBin[li[i]] = 1;
                binIdx[li[i]] = nDB;
                idxBin[nDB] = li[i];

                nDB++;
            }

            itemIdx[li[i]] = nDI;
            idxItem[nDI] = li[i];

            nDI++;
        }
    } // all items

    StrV *vnames = strv_create(80);

    strv_free(&vnames);


    free( binIdx );
    free( ivBin );
    lp_free(&mip);
}

