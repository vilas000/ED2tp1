#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SIZE_DADO2 1001
#define M 4
#define MM (M * 2)
#define SIZE_DADO3 5001
#define QUANT_REG 100
#define read_node(i, Item)     (!fseek(arvoreB, i * sizeof(TipoPaginaBin), SEEK_SET)) && (fread(Item, sizeof(TipoPaginaBin), 1, arvoreB) > 0)
typedef struct{
    unsigned int chave;
    long int dado1;
    char dado2[SIZE_DADO2];
    char dado3[SIZE_DADO3];
}Item;

typedef struct TipoPaginaBin* TipoApontadorBin;
typedef struct TipoPagina* TipoApontador;

typedef struct TipoPagina{
    short n;
    Item registros[MM];
    TipoApontador apontadores[MM + 1];
    bool ehFolha;
}TipoPagina;

typedef struct TipoPaginaBin{
    short n;
    Item registros[MM];
    int apontadores[MM + 1];
}TipoPaginaBin;

void inicializaArvoreBin(TipoApontadorBin *Arvore){
    *Arvore = NULL;
}

void inicializaArvore(TipoApontador *Arvore){
    *Arvore = NULL;
}

bool pesquisa(Item *x, FILE* arvoreB){

    TipoPaginaBin temp;
    long pos = 0;
    long i = 1;
    if(!read_node(pos, &temp)){
        //arvore vazia
        return false;
    }
    rewind(arvoreB);
    // long i = 1;

    while (read_node(pos, &temp)){
        while (i < temp.n && x->chave > temp.registros[i - 1].chave) 
            i++;
        if(x->chave == temp.registros->chave){
            *x = temp.registros[i - 1];
            return true;
        }
        else if(x->chave < temp.registros[i - 1].chave) i--;
            fseek(arvoreB, temp.apontadores[i] * sizeof(TipoPaginaBin), SEEK_SET); 
    }
    
    return false;


    // if(arvore == NULL)
    //     return false;

    // while(i < arvore->n && x->chave > arvore->registros[i - 1].chave) 
    //     i++;

    // if(x->chave == arvore->registros[i - 1].chave){
    //     *x = arvore->registros[i - 1];
    //     return true;
    // }

    // if(x->chave < arvore->registros[i - 1].chave)
    //     return pesquisa(x, arvore->apontadores[i - 1]);
    // else    
    //     return pesquisa(x, arvore->apontadores[i]);
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
        insereNaPagina(pArvoreTemp, *regRetorno, *pArvoreRetorno);
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
    TipoPagina *pRetorno, *pTemp;
    Ins(Reg, *pArvore, &cresceu, &itemRetorno, &pRetorno);
    if(cresceu){ // Arvore cresce na altura pela raiz
        pTemp = (TipoPagina*)malloc(sizeof(TipoPagina));
        pTemp->n = 1;
        pTemp->registros[0] = itemRetorno;
        pTemp->apontadores[1] = pRetorno;
        pTemp->apontadores[0] = *pArvore;
        pTemp->ehFolha = false;
        *pArvore = pTemp;
    }
}

void caminhamento(FILE* arquivo, TipoApontador *apontador, TipoApontadorBin apontadorBin, long i){
    
    if(apontador == NULL)
        return;

    rewind(arquivo);
    
    TipoApontador** filaOriginal = (TipoApontador**)malloc(QUANT_REG * sizeof(TipoApontador*));
    int frente = 0;
    int final = 0;

    filaOriginal[final++] = apontador;

    while(frente < final){
        TipoApontador* atual = filaOriginal[frente++];

        //gravacao em arquivo binario
        apontadorBin->apontadores[0] = i; i++;
        apontadorBin->n = (*atual)->n;
        for(int x = 0; x < (*atual)->n; x++){
            apontadorBin->apontadores[x + 1] = i; i++;
            apontadorBin->registros[x] = (*atual)->registros[x];
        }
        printf("%u\n", apontadorBin->registros->chave);
        fwrite(apontadorBin, sizeof(TipoApontadorBin), 1, arquivo);

        if(!(*atual)->ehFolha){
            for(int y = 0; y <= (*atual)->n; y++){
                if((*atual)->apontadores[y] != NULL){
                    filaOriginal[final++] = (&(*atual)->apontadores[y]);
                }
            }
        }

    }
    free(filaOriginal); 
}

int main(){

    FILE *arq;
    Item item;
    // TipoApontador apontador;
    // TipoApontadorBin apontadorBin = NULL;

    // if((arq = fopen("registrosDesordenadosCem.bin", "rb")) == NULL){
    //     perror("Erro na abertura do arquivo\n");
    //     exit(1);
    // } 
    //criacao da arvore B
    // inicializaArvore(&apontador);

    //insercao de elementos na arvore
    // for(int i = 0; fread(&item, sizeof(Item), 1, arq) > 0; i++)
    //     insere(item, &apontador);

    if((arq = fopen("arvoreBCem.bin", "rb")) == NULL){
        perror("Erro na abertura do arquivo\n");
        exit(1);
    } 
    
    //caminhamento(arq2, &apontador, apontadorBin, 1);

    printf("Digite o chave do regitro desejado: ");
    scanf("%d", &item.chave);

    if(pesquisa(&item, arq)){
        printf("\nChave: %u", item.chave);
        printf("\nDado 1: %ld", item.dado1);
        printf("\nDado 2: %s", item.dado2);
        printf("\nDado 3: %s", item.dado3);       
    } else 
        printf("Item nao encontrado");
    
    fclose(arq);

    return 0;
}