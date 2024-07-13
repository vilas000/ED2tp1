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
        struct { // U0: INTERNA
            int nInt;
            TipoChave rInt[M * 2];
            TipoApontador pInt[(M * 2) + 1];
        }U0;
        struct{ //U1: EXTERNA
            int nExt;
            Item rExt[M * M * 2];
        }U1;
    }UU;
}TipoPagina;

void inicializaArvore(TipoApontador *Arvore){
    *Arvore = NULL;
}

bool pesquisa(Item *x, TipoApontador *pArvore){
    printf("Searching for key: %u\n", x->chave);
    if(*pArvore == NULL)
        return false;

    TipoApontador pagina = *pArvore;
    int i;

    //pesquisa em paginas internas (U0)
    if(pagina->pt == Interna){
        i = 1;
        while(i < pagina->UU.U0.nInt && x->chave > pagina->UU.U0.rInt[i - 1]){
            i++;
        }
        if(x->chave < pagina->UU.U0.rInt[i - 1]) i--;
        return pesquisa(x, &pagina->UU.U0.pInt[i]);
    } else {
        //pesquisa sequencial na pagina folha
        i = 1;
        while(i < pagina->UU.U1.nExt && x->chave > pagina->UU.U1.rExt[i - 1].chave)
            i++;
        if(x->chave == pagina->UU.U1.rExt[i - 1].chave){
            *x = pagina->UU.U1.rExt[i - 1];
            return true;
        } else    
            return false;
    }
}

// void Imprime(TipoApontador arvore){
//     int i = 0;
//     if (arvore->pt == Externa) 
//         printf("key: %u\n", arvore->UU.U1.rExt[i].chave); return;
//     while (i <= arvore->UU.U0.nInt) {
//         Imprime(arvore->UU.U0.pInt[i]);
//         printf("dsadsa ");
//         if (i != arvore->UU.U1.nExt)
//     i++;
// }
// }

void insereNaPaginaExterna(TipoApontador pArvore, Item Reg){
    bool naoAchouPosicao;
    int k;

  //Pagina Externa (U1)
    k = pArvore->UU.U1.nExt; 
    naoAchouPosicao = (k > 0);

    while(naoAchouPosicao){
        if(Reg.chave >= pArvore->UU.U1.rExt[k - 1].chave){
            naoAchouPosicao = false;
            break;
        }
        pArvore->UU.U1.rExt[k] = pArvore->UU.U1.rExt[k - 1]; 
        //pArvore->apontadores[k + 1] = pArvore->apontadores[k];
        k--;
        if(k < 1) naoAchouPosicao = false;
    }
    pArvore->UU.U1.rExt[k] = Reg;
    //pArvore->apontadores[k + 1] = pDireita;
    pArvore->UU.U1.nExt++;
}

void insereNaPaginaInterna(TipoApontador pArvore, TipoChave chave, TipoApontador pDireita){
    bool naoAchouPosicao;
    int k;

 //Pagina Interna (U0)

    k = pArvore->UU.U0.nInt;
    naoAchouPosicao = (k > 0);

    while(naoAchouPosicao){
        if(chave >= pArvore->UU.U0.rInt[k - 1]){ 
            naoAchouPosicao = false;
            break;
        }
        pArvore->UU.U0.rInt[k] = pArvore->UU.U0.rInt[k - 1];
        pArvore->UU.U0.pInt[k + 1] = pArvore->UU.U0.pInt[k];
        k--;
        if(k < 1) naoAchouPosicao = false;
    }
    pArvore->UU.U0.rInt[k] = chave;
    pArvore->UU.U0.pInt[k + 1] = pDireita;
    pArvore->UU.U0.nInt++;
}

void arvoreDecrementa(TipoApontador *pArvore){
    if((*pArvore)->pt == Interna) //Interna: U0
        (*pArvore)->UU.U0.nInt--;
    else //Externa: U1
        (*pArvore)->UU.U1.nExt--;
}

void Ins(Item Reg, TipoApontador pArvore, bool *cresceu, TipoChave *regRetorno, Item *itemRetorno, TipoApontador *pArvoreRetorno){
    TipoApontador pArvoreTemp;
    int i = 1;
    //arvore nao existe OU no folha encontrado
    if(pArvore == NULL){
        *cresceu = true;
        *itemRetorno = Reg;
        *regRetorno = Reg.chave;
        *pArvoreRetorno = NULL;
        return;
    }

    if(pArvore->pt == Interna){ //(Interna: U0)
        i = 1;
        while(i < pArvore->UU.U0.nInt && Reg.chave > pArvore->UU.U0.rInt[i - 1])
            i++;

        if(Reg.chave < pArvore->UU.U0.rInt[i - 1]) i--;
        
        Ins(Reg, pArvore->UU.U0.pInt[i], cresceu, regRetorno, itemRetorno, pArvoreRetorno);

    if(!*cresceu) return;

    if(pArvore->UU.U0.nInt < (M * 2)){ //pagina tem espaco
        insereNaPaginaInterna(pArvore, *regRetorno, *pArvoreRetorno);
        *cresceu = false;
        return;
    }
    // Overflow: pagina tem que ser dividida
    pArvoreTemp = (TipoApontador)malloc(sizeof(TipoPagina));
    pArvoreTemp->pt = Interna;
    pArvoreTemp->UU.U0.nInt = 0;
    pArvoreTemp->UU.U0.pInt[0] = NULL;

    if(i < M + 1){
        insereNaPaginaInterna(pArvoreTemp, pArvore->UU.U0.rInt[(M * 2) - 1], pArvore->UU.U0.pInt[M * 2]);
        pArvoreTemp->UU.U0.nInt--; // decrementa 1 do valor de n
        insereNaPaginaInterna(pArvoreTemp, *regRetorno, *pArvoreRetorno);
    }
    else insereNaPaginaInterna(pArvoreTemp, *regRetorno, *pArvoreRetorno);

    for(int j = M + 2; j <= (M * 2); j++){
        insereNaPaginaInterna(pArvoreTemp, pArvore->UU.U0.rInt[j - 1], pArvore->UU.U0.pInt[j]);
        //pArvore->n--;
    }
        pArvore->UU.U0.nInt = M;
        //pArvoreTemp->UU.U0.pInt[0] = pArvore->UU.U0.pInt[M + 1];
        //pArvore->pt = Interna;
        //pArvoreTemp->pt = Externa;
        *regRetorno = pArvore->UU.U0.rInt[M];
        *pArvoreRetorno = pArvoreTemp; 
        return;
    }

    //if(pArvore->pt == Externa){
        //(Externa: U1)
        i = 1;
        while (i < pArvore->UU.U1.nExt && Reg.chave > pArvore->UU.U1.rExt[i - 1].chave)
            i++;
    // if (Reg.chave == pArvore->UU.U1.rExt[i - 1].chave) {
    //     *cresceu = false;
    //     return;
    // }

    if(pArvore->UU.U1.nExt < (M * 2)){ //pagina tem espaco

        insereNaPaginaExterna(pArvore, Reg);
        *cresceu = false;
        return;
    }

        // Overflow: pagina tem que ser dividida
        pArvoreTemp = (TipoApontador)malloc(sizeof(TipoPagina));
        pArvoreTemp->pt = Externa;
        pArvoreTemp->UU.U1.nExt = 0;
        //pArvoreTemp->apontadores[0] = NULL; 

        //determina em qual das duas paginas sera inserida o novo elemento 
        if(i < M + 1){
            insereNaPaginaExterna(pArvoreTemp, pArvore->UU.U1.rExt[(M * 2) - 1]);
            pArvoreTemp->UU.U1.nExt--; // decrementa 1 do valor de n
            insereNaPaginaExterna(pArvoreTemp, *itemRetorno);
        } 
        else insereNaPaginaExterna(pArvoreTemp, *itemRetorno);

        for(int j = M + 2; j <= (M * 2); j++){
            insereNaPaginaExterna(pArvoreTemp, pArvore->UU.U1.rExt[j - 1]);
            //pArvore->n--;
        }
            pArvore->UU.U1.nExt = (M + 1);
            //pArvoreTemp->UU.U0.pInt[0] = pArvore->UU.U0.pInt[M + 1];
            // pArvore->pt = Interna;
            // pArvoreTemp->pt = Externa;
            *itemRetorno = pArvore->UU.U1.rExt[M];
            *regRetorno = pArvore->UU.U1.rExt[M].chave;
            *pArvoreRetorno = pArvoreTemp;        
    //}

}

void insere(Item Reg, TipoApontador *pArvore){
    bool cresceu;
    Item itemRetorno;
    TipoChave chaveRetorno;
    TipoApontador pRetorno, pTemp;
    Ins(Reg, *pArvore, &cresceu, &chaveRetorno, &itemRetorno, &pRetorno);
    
    if(cresceu){ // Arvore cresce na altura pela raiz (Interna U0)
        pTemp = (TipoPagina*)malloc(sizeof(TipoPagina));
        
        if(*pArvore == NULL){ //primeiro elemento (Externa: U1)
            pTemp->pt = Externa;
            pTemp->UU.U1.nExt = 1;
            pTemp->UU.U1.rExt[0] = itemRetorno;
            //pTemp->UU.U0.pInt[0] = *pArvore;
            //pTemp->UU.U0.pInt[1] = pRetorno;
            //*pArvore = pTemp;
        } else {
            pTemp->pt = Interna;
            pTemp->UU.U0.nInt = 1;
            pTemp->UU.U0.rInt[0] = itemRetorno.chave;
            pTemp->UU.U0.pInt[0] = *pArvore;
            pTemp->UU.U0.pInt[1] = pRetorno;
        }
        *pArvore = pTemp;
    }
}

int main(){

    FILE *arq;
    Item item;
    TipoApontador apontador;

    if((arq = fopen("registrosDesordenadosCem.bin", "rb")) == NULL){
        perror("Erro na abertura do arquivo\n");
        exit(1);
    } 

    //criacao da arvore B
    inicializaArvore(&apontador);

    printf("teste 1\n");
    //insercao de elementos na arvore
    while(fread(&item, sizeof(Item), 1, arq) > 0){
        insere(item, &apontador);
        printf("Inserting key: %u\n", item.chave);
    }

    printf("teste 2\n");
    //pesquisa por elementos da arvore

    printf("Qual a chave do registro que deseja? ");
    scanf("%u", &item.chave);

    // Imprime(apontador);
    if(pesquisa(&item, &apontador)){
        printf("\nChave: %u", item.chave);
        printf("\nDado 1: %ld", item.dado1);
        printf("\nDado 2: %s", item.dado2);
        printf("\nDado 3: %s", item.dado3);
    } else {

        printf("Item nao encontrado");
    }

    
    fclose(arq);

    return 0;   
}