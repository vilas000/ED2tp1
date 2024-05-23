#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <stdbool.h>

#define SIZE_DADO2 1001
#define SIZE_DADO3 5001
#define QUANT_REG 100

#define ITENS_PAGINA 5
#define MAX_TABELA 100

typedef struct{
    unsigned int chave;
    long int dado1;
    char dado2[SIZE_DADO2];
    char dado3[SIZE_DADO3];
}Item;

bool pesquisa(unsigned int* tabelaChave, Item* item, long int chave, FILE* arq);

int main(int argc, char *argv[])
{
    unsigned int tabelaChave[MAX_TABELA];
    Item item[ITENS_PAGINA];
    FILE *arq;
    long int c;

    if((arq = fopen("registrosOrdenadosCem.bin", "rb")) == NULL){
        perror("Erro na abertura do arquivo\n");
        exit(1);
    } 

    //criacao da tabela de indices com chave
    for(int i = 0; fread(item, sizeof(Item), 5, arq) > 0; i++){
        tabelaChave[i] = item[0].chave;  
    }
    
    //inicio da pesquisa
    //fflush(arq);
    printf("Chave desejada: ");
    scanf("%ld", &c); //chave 

    if(pesquisa(tabelaChave, item, c, arq)){
        printf("\nChave: %u", item->chave);
        printf("\nDado 1: %ld", item->dado1);
        printf("\nDado 2: %s", item->dado2);
        printf("\nDado 3: %s", item->dado3);
    }

    fclose(arq);
    return 0;
}

bool pesquisa(unsigned int* tabelaChave, Item* item, long int chave, FILE* arq){
    rewind(arq);

    long int desloc;
    int quantItens;
    Item paginaEscolhida[ITENS_PAGINA];
    int i = 0; int tam = MAX_TABELA;
    while(i < tam && tabelaChave[i] <= chave) i++;
    int meio; int limInf = 0; int limSup;

    if(i == 0) return false; 
    else{

        if(i < tam) quantItens = ITENS_PAGINA;
        else{ //Chave se encontra na ultima pagina
            fseek(arq, 0, SEEK_END); //Ir pra o final do arquivo
            quantItens = (ftell(arq) / sizeof(Item)) % ITENS_PAGINA;
        }   
        desloc = sizeof(Item) * ITENS_PAGINA * (i - 1);
        fseek (arq, desloc, SEEK_SET);
        fread(paginaEscolhida, sizeof(Item), quantItens, arq);
        
        limSup = quantItens - 1;

        while(limInf <= limSup){
            meio = (limInf + limSup) / 2;

            if(paginaEscolhida[meio].chave == chave){
                *item = paginaEscolhida[meio];
                return true;
            }

            if(paginaEscolhida[meio].chave < chave)
                limInf = meio + 1;
            else   
                limSup = meio - 1;
        }

        return false;
    }
}