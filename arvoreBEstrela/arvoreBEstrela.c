#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define M 2
#define MM (M * 2)
#define SIZE_DADO2 1001
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

// * estrutura polimórfica
typedef struct TipoPagina{

    TipoIntExt tipo;
    int n;
    union {
        struct { 
            TipoChave rInt[M * 2];
            TipoApontador pInt[(M * 2) + 1];
        };
        struct {        
            Item rExt[2 * M];
        };
    };
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

    //pesquisa em paginas s ( )
    if(pagina->tipo == Interna){
        i = 1;
        while(i < pagina->n && x->chave > pagina->rInt[i - 1]){
            printf("NUMEROS NA INTERIOR: %ld\n", pagina->rInt[i - 1]);
            i++;
        }
        if(x->chave < pagina->rInt[i - 1]) i--;
        return pesquisa(x, &pagina->pInt[i]);
    } else if(pagina->tipo == Externa) {
        //pesquisa sequencial na pagina folha
        printf("TESTE 3\n");
        i = 1;
        while(i < pagina->n && x->chave > pagina->rExt[i - 1].chave){
            printf("NUMEROS NA FOLHA: %u\n", pagina->rExt[i - 1].chave);
            i++;
        }
        if(x->chave == pagina->rExt[i - 1].chave){
            *x = pagina->rExt[i - 1];
            return true;
        } else    
            return false;
    } else {
        printf("DEU RUIM ZE");
        return false;
    }
}

// void Imprime(TipoApontador arvore){
//     int i = 0;
//     if (arvore-> == 
//         printf("key: %u\n", arvore->rExt[i].chave); return;
//     while (i <= arvore-> n) {
//         Imprime(arvore->pInt[i]);
//         printf("dsadsa ");
//         if (i != arvore->n)
//     i++;
// }
// }

void insereNaPaginaExterna(TipoApontador pArvore, Item Reg){
    bool naoAchouPosicao;
    int k;

  //Pagina(
    k = pArvore->n; 
    naoAchouPosicao = (k > 0);

    while(naoAchouPosicao){
        if(Reg.chave >= pArvore->rExt[k - 1].chave){
            naoAchouPosicao = false;
            break;
        }
        // pArvore->rExt[k] = pArvore->rExt[k - 1]; ANTIGO
        (*pArvore).rExt[k] = (*pArvore).rExt[k - 1];  
        k--;
        if(k < 1) naoAchouPosicao = false;
    }
    // pArvore->rExt[k] = Reg; ANTIGO
    // pArvore->n++; ANTIGO
    (*pArvore).rExt[k] = Reg;
    (*pArvore).n++;
}

void insereNaPaginaInterna(TipoApontador pArvore, TipoChave chave, TipoApontador pDireita){
    bool naoAchouPosicao;
    int k;

    k = pArvore-> n;
    naoAchouPosicao = (k > 0);

    while(naoAchouPosicao){
        if(chave >= pArvore->rInt[k - 1]){ 
            naoAchouPosicao = false;
            break;
        }
        // pArvore->rInt[k] = pArvore->rInt[k - 1]; ANTIGO
        // pArvore->pInt[k + 1] = pArvore->pInt[k];
        (*pArvore).rInt[k] = (*pArvore).rInt[k - 1];
        (*pArvore).pInt[k + 1] = (*pArvore).pInt[k];
        k--;
        if(k < 1) naoAchouPosicao = false;
    }
    // pArvore->rInt[k] = chave;
    // pArvore->pInt[k + 1] = pDireita;
    // pArvore->n++; ANTIGOS
    (*pArvore).rInt[k] = chave;
    (*pArvore).pInt[k + 1] = pDireita;
    (*pArvore).n++;
}

void arvoreDecrementa(TipoApontador *pArvore){
    (*pArvore)->n --;
}

void Ins(Item Reg, TipoApontador pArvore, bool *cresceu, TipoChave *regRetorno, Item *itemRetorno, TipoApontador *pArvoreRetorno){
    long i = 1;
    TipoApontador pArvoreTemp;
    //arvore nao existe OU no folha encontrado
    if(pArvore == NULL){
        *cresceu = true;
        *itemRetorno = Reg;
        *regRetorno = Reg.chave;
        *pArvoreRetorno = NULL;
        fprintf(stderr, ">>>\t[%s:%03d] feijao\n", __func__, __LINE__);
        return;
    }

    if(pArvore->tipo == Interna){ 
        i = 1;
        while(i < pArvore->n && Reg.chave > pArvore->rInt[i - 1])
            i++;

        if(Reg.chave < pArvore->rInt[i - 1]) i--;
        
        Ins(Reg, pArvore->pInt[i], cresceu, regRetorno, itemRetorno, pArvoreRetorno);

    if(!*cresceu) return;

    if(pArvore->n < (M * 2)){ //pagina tem espaco
        insereNaPaginaInterna(pArvore, *regRetorno, *pArvoreRetorno);
        *cresceu = false;
        return;
    }
    // Overflow: pagina tem que ser dividida
    pArvoreTemp = (TipoApontador)malloc(sizeof(TipoPagina));
    pArvoreTemp->tipo = Interna;
    pArvoreTemp-> n = 0;
    // pArvoreTemp->pInt[0] = NULL;
    
    if(i < M + 1){
        insereNaPaginaInterna(pArvoreTemp, pArvore->rInt[(M * 2) - 1], pArvore->pInt[M * 2]);
        pArvore->n--; // decrementa 1 do valor de n
        insereNaPaginaInterna(pArvoreTemp, *regRetorno, *pArvoreRetorno);
    }
    else insereNaPaginaInterna(pArvoreTemp, *regRetorno, *pArvoreRetorno);
    
    for(int j = M + 2; j <= (M * 2); j++){
        insereNaPaginaInterna(pArvoreTemp, pArvore->rInt[j - 1], pArvore->pInt[j]);
    }
        pArvore-> n = M;
        pArvoreTemp->n = M;
        pArvore->tipo = Interna;
        // pArvoreTemp->tipo = Externa;
        pArvoreTemp->pInt[0] = pArvore->pInt[M + 1];
        *regRetorno = pArvore->rInt[M];
        *pArvoreRetorno = pArvoreTemp; 
        *cresceu = true;
        return;
    }

    //Externa

    i = 1;
    while (i < pArvore->n && Reg.chave > pArvore->rExt[i - 1].chave)
        i++;

    if(pArvore->n < (M * 2)){ //pagina tem espaco

        insereNaPaginaExterna(pArvore, *itemRetorno);
        printf("qnt de itens: %d\n", pArvore->n);
        *cresceu = false;
        return;
    }

        // Overflow: pagina tem que ser dividida
        pArvoreTemp = (TipoApontador)malloc(sizeof(TipoPagina));
        pArvoreTemp->n = 0;
        pArvoreTemp->tipo = Externa;

        //determina em qual das duas paginas sera inserida o novo elemento 
        if(i < M + 1){
            insereNaPaginaExterna(pArvoreTemp, pArvore->rExt[(M * 2) - 1]);
            pArvore->n--; // decrementa 1 do valor de n
            insereNaPaginaExterna(pArvore, *itemRetorno);
        } 
        else insereNaPaginaExterna(pArvoreTemp, *itemRetorno);
//CHAVE 236992
        for(int j = M + 1; j <= (M * 2); j++){
            insereNaPaginaExterna(pArvoreTemp, pArvore->rExt[j - 1]);
        }
            pArvore->n = M;
            pArvoreTemp->n = M + 1;
            //pArvoreTemp->pInt[0] = pArvore->pInt[M + 1];
            // pArvore-> = ;
            // pArvoreTemp-> =
            // pArvore->tipo = Externa;
            //pArvoreTemp->tipo = Externa;
            *itemRetorno = pArvore->rExt[M];
            *regRetorno = pArvore->rExt[M].chave;
            *pArvoreRetorno = pArvoreTemp;   
            *cresceu = true;
            return;     
    //}
}

void insere(Item Reg, TipoApontador *pArvore){
    bool cresceu;
    Item itemRetorno;
    TipoChave chaveRetorno;
    TipoApontador pRetorno, pTemp;

    //TipoPagina* pTemp = { 0 };

    Ins(Reg, *pArvore, &cresceu, &chaveRetorno, &itemRetorno, &pRetorno);
    
    if(cresceu){ // Arvore cresce na altura pela raiz (  )
        pTemp = (TipoPagina*)malloc(sizeof(TipoPagina));
        if(*pArvore == NULL){ //primeiro elemento  
                
            pTemp->tipo = Externa;
            pTemp->n = 1;
            pTemp->rExt[0] = itemRetorno;
            // fprintf(stderr, ">>>\t[%s:%03d] feijoada\n", __func__, __LINE__);
            // printf("[ ");
            // for (size_t i = 0; i < temp.n; i ++)
            //     printf("(%d), ", temp.rExt[i].chave);
            // printf("]\n");
        
        } else {
            pTemp->tipo = Interna;
            pTemp->n = 1;
            pTemp->rInt[0] = chaveRetorno;
            pTemp->pInt[0] = *pArvore;
            pTemp->pInt[1] = pRetorno;
        }
        *pArvore = pTemp;
        //chave: 552923
        // fprintf(stderr, ">>>\t[%s:%03d] feijoada doida\n", __func__, __LINE__);
        // printf("[ ");
        // for (size_t i = 0; i < temp.n; i ++)
        //     printf("(%d), ", temp.rExt[i].chave);
        // printf("]\n");
    }
}
static bool inline read_bs(size_t _Index, FILE * _Stream, struct TipoPagina * _Buffer) { 
    return (!fseek(_Stream, _Index * sizeof(struct TipoPagina), SEEK_SET)) && (fread(_Buffer, sizeof(struct TipoPagina), 1, _Stream) > 0); 
}

// static void print_bs(FILE* bs_stream)
// {   
//     rewind(bs_stream);
//     struct TipoPagina buffer; 
//     size_t iterator = 0;
//     if (read_bs((iterator ++), bs_stream, &buffer))
//     {
//         printf("[%lu]: (%d) ", (unsigned long) (iterator - 1), (int) buffer.n);
//         if (buffer.tipo == 0) {
//             size_t i = 0;
//             printf("[ ");
//             for (; i < buffer.n; i ++)
//                 printf("(*%ld), ", buffer.rInt[i]);
//             printf("] & [ ");
//             for (i = 0; i <= buffer.n; i ++)
//                 printf("(%p) ", buffer.pInt[i]);
//             printf("]");
//         } else {
//             printf("[ ");
//             for (size_t i = 0; i < buffer.n; i ++)
//                 printf("(%d), ", buffer.rExt[i].chave);
//             printf("] & [ ");
//         }
//         printf("\n");
//     }
// }

// bool insereNovo(Item Reg, TipoApontador *pArvore, long int qtt){
//     TipoPagina* temp = { 0 };
//     if(qtt == 0){
//         // Caso verdadeiro, isso significa que o arquivo está vazio, então podemos inserir direto
//         temp->tipo = Externa;
//         temp->n = 1;
//         temp->rExt;
//     }
// }

int main(){

    FILE *arq;
    Item item;
    TipoApontador apontador = NULL;
    int cont = 1;

    if((arq = fopen("registrosDesordenadosCem.bin", "rb")) == NULL){
        perror("Erro na abertura do arquivo\n");
        exit(1);
    } 

    //criacao da arvore B
    inicializaArvore(&apontador);

    printf("teste 1\n");
    //insercao de elementos na arvore
    while(fread(&item, sizeof(Item), 1, arq) > 0){
        printf("Inserting key %d: %u\n", cont, item.chave);
        cont++;

        insere(item, &apontador);
        
        // print_bs(arq);
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