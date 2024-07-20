#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SIZE_DADO2 1001
#define M 4
#define MM (M * 2)
#define SIZE_DADO3 5001
#define QUANT_REG 100

typedef struct{
    unsigned int chave;
    long int dado1;
    char dado2[SIZE_DADO2];
    char dado3[SIZE_DADO3];
}Item;

typedef struct TipoPagina* TipoApontador;

typedef struct TipoPagina{
    short n;
    Item registros[MM];
    TipoApontador apontadores[MM + 1];
    bool ehFolha;
}TipoPagina;

void inicializaArvore(TipoApontador *Arvore){
    *Arvore = NULL;
}

bool pesquisa(Item *x, TipoApontador arvore){

    long i = 1;
    
    if(arvore == NULL)
        return false;

    while(i < arvore->n && x->chave > arvore->registros[i - 1].chave) 
        i++;

    if(x->chave == arvore->registros[i - 1].chave){
        *x = arvore->registros[i - 1];
        return true;
    }

    if(x->chave < arvore->registros[i - 1].chave)
        return pesquisa(x, arvore->apontadores[i - 1]);
    else    
        return pesquisa(x, arvore->apontadores[i]);
}

void insereNaPagina(TipoApontador pArvore, Item Reg, TipoApontador pDireita){
    bool naoAchouPosicao;
    int k;
    k = pArvore->n; 
    naoAchouPosicao = (k > 0);

    while(naoAchouPosicao){
        if(Reg.chave >= pArvore->registros[k - 1].chave){
            naoAchouPosicao = false;
            break;
        }
        pArvore->registros[k] = pArvore->registros[k - 1];
        pArvore->apontadores[k + 1] = pArvore->apontadores[k];
        k--;
        if(k < 1) naoAchouPosicao = false;
    }
    pArvore->registros[k] = Reg;
    pArvore->apontadores[k + 1] = pDireita;
    (pArvore->n)++;
}

void Ins(Item Reg, TipoApontador pArvore, bool *cresceu, Item *regRetorno, TipoApontador *pArvoreRetorno){
    long i = 1;
    TipoApontador pArvoreTemp;
    //arvore nao existe OU no folha encontrado
    if(pArvore == NULL){
        *cresceu = true;
        (*regRetorno) = Reg;
        (*pArvoreRetorno) = NULL;
        return;
    }

    while(i < pArvore->n && Reg.chave > pArvore->registros[i - 1].chave) i++;

    if(Reg.chave ==  pArvore->registros[i - 1].chave){
        printf("Registro ja presente\n");
        *cresceu = false;
        return;
    }

    if(Reg.chave < pArvore->registros[i - 1].chave) i--; //saber para onde caminhar
    Ins(Reg, pArvore->apontadores[i], cresceu, regRetorno, pArvoreRetorno);
   
    if(!*cresceu) return;

    if(pArvore->n < (M * 2)){ //pagina tem espaco
        insereNaPagina(pArvore, *regRetorno, *pArvoreRetorno);
        *cresceu = false;
        return;
    }

    // Overflow: pagina tem que ser dividida
    pArvoreTemp = (TipoApontador)malloc(sizeof(TipoPagina));
    pArvoreTemp->n = 0;
    pArvoreTemp->apontadores[0] = NULL;

    //determina em qual das duas paginas sera inserida o novo elemento 
    if(i < M + 1){
        insereNaPagina(pArvoreTemp, pArvore->registros[(M * 2) - 1], pArvore->apontadores[M * 2]);
        (pArvore->n)--;
        insereNaPagina(pArvore, *regRetorno, *pArvoreRetorno);
    }
    else insereNaPagina(pArvoreTemp, *regRetorno, *pArvoreRetorno);

    for(int j = M + 2; j <= (M * 2); j++){
        insereNaPagina(pArvoreTemp, pArvore->registros[j - 1], pArvore->apontadores[j]);
        //pArvore->n--;
    }
        pArvore->n = M;
        pArvoreTemp->apontadores[0] = pArvore->apontadores[M + 1];
        pArvore->ehFolha = false;
        pArvoreTemp->ehFolha = true;
        *regRetorno = pArvore->registros[M];
        *pArvoreRetorno = pArvoreTemp;        
}

void insere(Item Reg, TipoApontador *pArvore){
    bool cresceu;
    Item itemRetorno;
    TipoApontador pRetorno, pTemp;
    Ins(Reg, *pArvore, &cresceu, &itemRetorno, &pRetorno);
    if(cresceu){ // Arvore cresce na altura pela raiz
        pTemp = (TipoPagina*)malloc(sizeof(TipoPagina));
        pTemp->n = 1;
        pTemp->registros[0] = itemRetorno;
        pTemp->apontadores[0] = *pArvore;
        pTemp->apontadores[1] = pRetorno;
        pTemp->ehFolha = false;
        *pArvore = pTemp;
    }
}

int main(void) {

    FILE *arq;
    Item item;
    TipoApontador apontador;
    
    if((arq = fopen("registrosDesordenadosCem.bin", "rb")) == NULL){
        perror("Erro na abertura do arquivo\n");
        exit(1);
    } 

    //criacao da arvore B
    inicializaArvore(&apontador);

    //insercao de elementos na arvore
    for(int i = 0; fread(&item, sizeof(Item), 1, arq) > 0; i++)
        insere(item, &apontador);

    //pesquisa por elementos da arvore

    printf("Qual a chave do registro que deseja? ");
    scanf("%u", &item.chave);
    
    if(pesquisa(&item, apontador)){
        printf("\nChave: %u", item.chave);
        printf("\nDado 1: %ld", item.dado1);
        printf("\nDado 2: %s", item.dado2);
        printf("\nDado 3: %s", item.dado3);
    } else 
        printf("Item nao encontrado");
    
    fclose(arq);

    return 0;
}