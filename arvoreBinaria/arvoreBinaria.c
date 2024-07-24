#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <unistd.h>

#define SIZE_DADO2 1001
#define SIZE_DADO3 5001
#define SIZE_MAX_NO 3
#define N_ITEM_PAG 3
//#define read_nodes(i, Item) (!fseek(Arquivo, (pag * N_ITEM_PAG) * sizeof(No), SEEK_SET)) && (fread(temp.no, sizeof(No), N_ITEM_PAG, Arquivo) > 0)


typedef struct Registro
{
    long chave;
    long int dado1;
    char dado2[SIZE_DADO2];
    char dado3[SIZE_DADO3];
} Registro;

typedef struct No
{
    Registro dadoNo;
    long direita;
    long esquerda;
} No;

typedef struct Pagina
{
    struct No no[N_ITEM_PAG];
} Pagina;

bool InsereRegistro(No R, FILE * Arquivo, const long qtt);
bool PesquisaRegistro(long chave, FILE * Arquivo, const long qtt, Registro * recebido);

int main()
{
    No R = { 0 };
    FILE *arquivo;
    FILE *arquivobinario;
    R.direita = -1;
    R.esquerda = -1;
    
    long qtt = 0;

    if ((arquivobinario = fopen("arvorebinaria.bin", "w+b")) == NULL)
    {
        printf("Erro na abertura do arquivo\n");
        exit(1);
    }

    if ((arquivo = fopen("arvoreteste.bin", "rb")) == NULL)
    {
        printf("Erro na abertura do arquivo\n");
        exit(1);
    }

    while (fread(&R.dadoNo, sizeof(Registro), 1, arquivo))
    {
        // printf("(Lendo) [%llu]: <%ld, %d, %ld>\n", (unsigned long long) qtt, R.esquerda, R.dadoNo.chave, R.direita);

        if (!InsereRegistro(R, arquivobinario, qtt))
        {
            printf("Falha ao inserir\n");
            continue;
        }
        qtt++;

        No tempMarcus;
        rewind(arquivobinario);
        while (fread(&tempMarcus, sizeof(No), 1, arquivobinario)) {
            printf("[%llu]: <%ld, %ld, %ld>\n", (unsigned long long) qtt,
                tempMarcus.esquerda, tempMarcus.dadoNo.chave, tempMarcus.direita);
        }
    }

    int pesquisa;
    Registro recebido;
    printf("\npesquisar uma item\nDigite uma chave:\n");
    scanf("%d", &pesquisa);
    if(PesquisaRegistro(pesquisa, arquivobinario, qtt ,&recebido)){
        printf("Pesquisa realizada com sucesso!\nDado 1: %ld\nDado 2: %s\nDado 3: %s\n", recebido.dado1, recebido.dado2, recebido.dado3);
    }
    else{
        printf("\nItem não encontrado\n");
    }
    
    fclose(arquivo);
    fclose(arquivobinario);
    return 0;
}

bool InsereRegistro(No R, FILE * Arquivo, const long qtt) {
    #define read_node(i, Item)     (! fseek(Arquivo, i * sizeof(No), SEEK_SET)) && (fread(Item, sizeof(No), 1, Arquivo) > 0)
    
    printf("[%s] (qtt:%ld)\n", __func__, qtt);

    if (qtt == 0)
    {
        // Caso verdadeiro, isso significa que o arquivo está vazio, então podemos inserir direto
        printf("Primeiro item inserido\n");
        fwrite(&R, sizeof(No), 1, Arquivo);
        return true;
        
    }
    
    No temp;
    long pos = 0;

    while (read_node(pos, &temp))
    {
        printf("[%s] qtt: %ld, pos=%ld, temp: <%ld, %ld, %ld>\n", __func__, qtt, pos, temp.esquerda, temp.dadoNo.chave, temp.direita);
        //usleep(1000 * 500);
        
        if (temp.dadoNo.chave == R.dadoNo.chave) {
            //caso verdadeiro retornamos que a chave ja esta presente e portanto nao foi inserida
            printf("A chave já está presente no arquivo\n");
            return false;

        } else if (temp.dadoNo.chave > R.dadoNo.chave) {
            if (temp.esquerda == -1) {
                temp.esquerda = qtt;
                fseek(Arquivo, pos * sizeof(No), SEEK_SET);
                fwrite(&temp, sizeof(No), 1, Arquivo);
                fseek(Arquivo, qtt * sizeof(No), SEEK_SET);
                fwrite(&R, sizeof(No), 1, Arquivo);
                return true;
            }
            pos = temp.esquerda;

        } else {
            if (temp.direita == -1) {
                temp.direita = qtt;
                fseek(Arquivo, pos * sizeof(No), SEEK_SET);
                fwrite(&temp, sizeof(No), 1, Arquivo);
                fseek(Arquivo, qtt * sizeof(No), SEEK_SET);
                fwrite(&R, sizeof(No), 1, Arquivo);
                return true;
            }
            pos = temp.direita;     
        }   
    }
    return false;
}

bool PesquisaRegistro(long chave, FILE * Arquivo, const long qtt, Registro * recebido) {
    if (qtt == 0) {
        printf("O arquivo esta vazio\n");
        return true;
    }
    
    #define read_page(_IndiceDaPagina, _PaginaPtr)     ((! fseek(Arquivo, _IndiceDaPagina * sizeof(struct Pagina), SEEK_SET)) && (fread(_PaginaPtr, sizeof(No), N_ITEM_PAG, Arquivo) > 0))

    Pagina temp;

    long pos = 0;
    size_t posItem = 0;
    size_t pag = 0;
    size_t old_pag = 1;

    while (true) {   
        
        if (old_pag != pag) {
            if (! read_page(pag, &temp))
                break;
        }
        
        if(chave == temp.no[posItem].dadoNo.chave) {
            * recebido = temp.no[posItem].dadoNo;
            return true;
        }
        
        else if (chave < temp.no[posItem].dadoNo.chave) {
            if (temp.no[posItem].esquerda == -1)    // acabou a pesquisa pela esquerda
                break;
                
            pos = temp.no[posItem].esquerda;

            // atualizando os parâmetros de navegação marcus vrum
            old_pag = pag;
            posItem = pos % N_ITEM_PAG;
            pag = pos / N_ITEM_PAG;
        } else {
            if (temp.no[posItem].direita == -1)     // acabou a pesquisa pela direita
                break;

            pos = temp.no[posItem].direita;
            
            // atualizando os parâmetros de navegação marcus vrum
            old_pag = pag;
            posItem = pos % N_ITEM_PAG;
            pag = pos / N_ITEM_PAG;
        }
    }
    return false;
}



/*
bool PesquisaRegistro(long chave, FILE *Arquivo, Registro *recebido, const long qtt){
    #define read_nodes(i, Item) (!fseek(Arquivo, pos * sizeof(No), SEEK_SET)) && (fread(temp.no, sizeof(No), N_ITEM_PAG, Arquivo) > 0)
    
    if (qtt == 0){
        printf("O arquivo esta vazio\n");
        return true;
    }

    Pagina temp;
    long pos = 0;
    int posItem = 0;
    
    while(read_nodes(pos, &temp)){ 
        //printf("\nchave posicao %ld: %ld\ndado1: %ld\ndado2: %s\ndado3: %s\n", pos, temp.no[pos].dadoNo.chave ,temp.no[pos].dadoNo.dado1, temp.no[pos].dadoNo.dado2, temp.no[pos].dadoNo.dado3);
        printf("\npos inicio: %ld\n", pos);
        int i = pos + N_ITEM_PAG;
        while(pos < i){
            printf("\nchave desejada:%ld \nchave lida: %ld\n", chave, temp.no[pos].dadoNo.chave);
            if(chave == temp.no[posItem].dadoNo.chave){
                printf("\nchave posicao %ld: %ld\ndado1: %ld\ndado2: %s\ndado3: %s\n", pos, temp.no[posItem].dadoNo.chave ,temp.no[posItem].dadoNo.dado1, temp.no[posItem].dadoNo.dado2, temp.no[posItem].dadoNo.dado3);
                recebido = &temp.no[posItem].dadoNo;
                return true;
            }
            else if (temp.no[pos].dadoNo.chave > chave && temp.no[pos].esquerda != -1){
                if(temp.no[posItem].esquerda < i){
                    posItem = temp.no[posItem].esquerda;
                    printf("\npos esquerda1: %ld", pos);
                    continue;
                }
                else{
                    pos = temp.no[posItem].esquerda;
                    pos = pos % N_ITEM_PAG;
                    printf("\npos esquerda2: %ld", pos);
                    break;
                }
            }
            else if(temp.no[posItem].dadoNo.chave < chave && temp.no[posItem].direita != -1){
                if(temp.no[posItem].direita < i){
                    posItem = temp.no[posItem].direita;
                    printf("\npos Direira1: %ld", pos);
                    continue;
                }
                else{
                    pos = temp.no[posItem].direita;
                    pos = pos % N_ITEM_PAG;
                    printf("\npos Direira2: %ld", pos);
                    break;
                }         
            }
            else{
                return false;
            }
        }
    }
    return false;
}
*/

/*
bool PesquisaRegistro(long chave, FILE *Arquivo, const long qtt){
    #define read_nodes(i, Item) (!fseek(Arquivo, (pag * N_ITEM_PAG) * sizeof(No), SEEK_SET)) && (fread(temp.no, sizeof(No), N_ITEM_PAG, Arquivo) > 0)
    if (qtt == 0){
        printf("O arquivo esta vazio\n");
        return true;
    }
    long pag = 0;
    Pagina temp;
    
    if(!read_nodes(pag, &temp)){
        printf("erro na leitura\n");
        return false;
    }
    
    long posItem = 0;
    int N_paginas = qtt /N_ITEM_PAG;
    int pos = 0;    
    int Pag_lidas = 1; 
    int i = N_ITEM_PAG;
    printf("\nPag_lidas: %d\nN_paginas: %d\n", Pag_lidas, N_paginas);
    while(Pag_lidas < N_paginas){   
        while (pos < i){
            printf("ok2\n");
            printf("\nchave posicao %ld: %ld\ndado1: %ld\ndado2: %s\ndado3: %s\n", posItem, temp.no[posItem].dadoNo.chave ,temp.no[posItem].dadoNo.dado1, temp.no[posItem].dadoNo.dado2, temp.no[posItem].dadoNo.dado3);
            if(chave == temp.no[posItem].dadoNo.chave){
                recebido = &temp.no[posItem].dadoNo;
                return true;
            }
            else if(chave < temp.no[posItem].dadoNo.chave && temp.no[posItem].esquerda != -1){
                if(temp.no[posItem].esquerda < i){
                    posItem = temp.no[posItem].esquerda % N_ITEM_PAG;
                    continue;
                }
                else{
                    pos = temp.no[posItem].esquerda;
                    posItem = pos % N_ITEM_PAG;
                    pag = pos / N_ITEM_PAG;
                    break;
                }
            }
            else if(chave < temp.no[posItem].dadoNo.chave && temp.no[posItem].direita != -1){
                if(temp.no[posItem].direita < i){
                    posItem = temp.no[posItem].direita % N_ITEM_PAG;
                    continue;
                }
                else{
                    pos = temp.no[posItem].direita;
                    posItem = pos % N_ITEM_PAG;
                    pag = pos / N_ITEM_PAG;
                    break;
                }
            }
            else{
                printf("\nalgum erro inesperado\n");
                return false;
            }

        }
        if(!read_nodes(pag, &temp)){
            printf("erro na leitura\n");
            return false;
        }       
        i +=  N_ITEM_PAG;
        Pag_lidas += 1;
    }
    return false;
}
*/
