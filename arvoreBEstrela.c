#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SIZE_DADO2 1001
#define M 4
#define SIZE_DADO3 5001
#define QUANT_REG 100

typedef struct{
    unsigned int chave;
    long int dado1;
    char dado2[SIZE_DADO2];
    char dado3[SIZE_DADO3];
}Item;

typedef long TipoChave;

typedef enum {Interna, Externa} TipoIntExt;

typedef struct TipoPagina* TipoApontador;

typedef struct TipoPagina{

    TipoIntExt pt;
    union {
        struct {
            int nInt;
            TipoChave rInt[M * 2];
            TipoApontador pInt[(M * 2) + 1];
        }U0;
        struct{ 
            int nExt;
            Item rExt[M * M * 2];
        }U1;
    }UU;
    
}TipoPagina;

bool pesquisa(Item *x, TipoApontador *pArvore){
    int i;
    TipoApontador pagina = *pArvore;

    //pesquisa em paginas internas (U0)
    if((*pArvore)->pt == Interna){
        i = 1;
        while(i < pagina->UU.U0.nInt && x->chave > pagina->UU.U0.rInt[i - 1]) 
            i++;
        
        if(x->chave > pagina->UU.U0.rInt[i - 1])
            pesquisa(x, &pagina->UU.U0.pInt[i]);
        else
            pesquisa(x, &pagina->UU.U0.pInt[i - 1]);
        return true;
    }
    i = 1;
    //pesquisa sequencial na pagina folha
    while(i < pagina->UU.U1.nExt && x->chave > pagina->UU.U1.rExt[i - 1].chave);
        i++;
    if(x->chave == pagina->UU.U1.rExt[i - 1].chave){
        *x = pagina->UU.U1.rExt[i - 1];
        return true;
    }
    else    
        return false;
}


int main(){

}