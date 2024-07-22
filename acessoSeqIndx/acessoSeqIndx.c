#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <stdbool.h>

#define SIZE_DADO2 1001
#define SIZE_DADO3 5001
#define QUANT_REG 1000000

#define ITENS_PAGINA 500
#define MAX_TABELA 1000
#define TAMANHO 10

typedef struct{
    unsigned int chave;
    long int dado1;
    char dado2[SIZE_DADO2];
    char dado3[SIZE_DADO3];
}Item;

bool pesquisa(unsigned int* tabelaChave, Item* item, FILE* arq, int *numTransferencias, int *numComparacoes);

int main(int argc, char *argv[])
{
    int vetor[TAMANHO];
    double somaTempo = 0;
    int somaComparacoes = 0;
    int somaTransferencias = 0;
    srand(time(NULL));

    // Gera 10 números aleatórios únicos no intervalo [0, n-1]
    for (int i = 0; i < TAMANHO; i++) {
        int numero;
        int unico;

        do {
            unico = 1;
            numero = rand() % QUANT_REG; // Gera um número aleatório no intervalo [0, n-1]

            // Verifica se o número já foi gerado
            for (int j = 0; j < i; j++) {
                if (vetor[j] == numero) {
                    unico = 0;
                    break;
                }
            }
        } while (!unico);

        vetor[i] = numero;
    }

    for (int p = 0; p < TAMANHO; p++) {
        unsigned int tabelaChave[MAX_TABELA];
        Item item[ITENS_PAGINA];
        FILE *arq;
        int numTransferencias = 0;
        int numComparacoes = 0;

        // Captura o tempo de início
        clock_t horaInicio = clock();

        if((arq = fopen("registrosOrdenadosCem.bin", "rb")) == NULL){
            perror("Erro na abertura do arquivo\n");
            exit(1);
        } 

        //criacao da tabela de indices com chave
        for(int i = 0; fread(item, sizeof(Item), ITENS_PAGINA, arq) > 0; i++){
            numTransferencias++;
            tabelaChave[i] = item[0].chave;  
        }
        
        //inicio da pesquisa
        //fflush(arq);
        item->chave = vetor[p];

        if(pesquisa(tabelaChave, item, arq, &numTransferencias, &numComparacoes)){
            //printf("\nexiste!!!");
            //printf("\nChave: %u", item->chave);
            /*
            printf("\nDado 1: %ld", item->dado1);
            printf("\nDado 2: %s", item->dado2);
            printf("\nDado 3: %s", item->dado3);
            */
        } 
        else {
            printf("Item %d nao existente", vetor[p]);
        }

        fclose(arq);

        // Captura o tempo de fim
        clock_t horaFim = clock();

        // Calcula o tempo total de execução
        double tempo = (double)(horaFim - horaInicio) / CLOCKS_PER_SEC;
        somaTempo += tempo;
        somaComparacoes += numComparacoes;
        somaTransferencias += numTransferencias;

        printf("\n%d & %d & %d & %.6f \\\\", vetor[p], numComparacoes, numTransferencias,tempo);
        /*
        printf("\nTempo total de execucao: %.6f segundos", tempo);
        printf("\nNúmero de comparações: %d", numComparacoes);
        printf("\nNúmero de transferências: %d \n", numTransferencias);
        */

    }
    /*
    printf("\nTempo medio de execucao: %.6f segundos", somaTempo/TAMANHO);
    printf("\nNúmero medio de comparações: %d", somaComparacoes/TAMANHO);
    printf("\nNúmero medio de transferências: %d \n", somaTransferencias/TAMANHO);
    */

    printf("\nMédia & %d & %d & %.6f\\\\\\hline\\hline\n", somaComparacoes/TAMANHO, somaTransferencias/TAMANHO, somaTempo/TAMANHO);

    return 0;
}

bool pesquisa(unsigned int* tabelaChave, Item* item, FILE* arq, int *numTransferencias, int *numComparacoes){
    rewind(arq);

    long int desloc;
    int quantItens;
    Item paginaEscolhida[ITENS_PAGINA];
    int i = 0; int tam = MAX_TABELA;
    while(i < tam && tabelaChave[i] <= item->chave) {
        i++;
        (*numComparacoes)++;
    }
    int meio; int limInf = 0; int limSup;
    if(i == 0) return false; 

    if(i < tam) quantItens = ITENS_PAGINA;
    else{ //Chave se encontra na ultima pagina
        fseek(arq, 0, SEEK_END); //Ir pra o final do arquivo
        quantItens = (ftell(arq) / sizeof(Item)) % ITENS_PAGINA;
        if (quantItens == 0) quantItens = ITENS_PAGINA;
    }   
    desloc = sizeof(Item) * ITENS_PAGINA * (i - 1);
    fseek (arq, desloc, SEEK_SET);
    fread(paginaEscolhida, sizeof(Item), quantItens, arq);
    (*numTransferencias)++;
    
    limSup = quantItens - 1;


    while(limInf <= limSup){
        meio = (limInf + limSup) / 2;

        if(paginaEscolhida[meio].chave == item->chave){
            *item = paginaEscolhida[meio];
            (*numComparacoes)++;
            return true;
        }

        if(paginaEscolhida[meio].chave < item->chave){
            limInf = meio + 1;
            (*numComparacoes)++;
        }
        else{   
            limSup = meio - 1;
        }
    }

    return false;
}
