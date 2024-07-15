#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SIZE_DADO2 1001
#define SIZE_DADO3 5001
#define TAM_PAG 5
#define read_node(i, Item)     (! fseek(Arquivo, i * sizeof(No), SEEK_SET)) && (fread(Item, sizeof(No), 1, Arquivo) > 0)

typedef struct Registro
{
    unsigned int chave;
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
    No *nos[TAM_PAG];
    long nNos;
    struct Pagina *proxima;
} Pagina;

// bool InsereRegistro(No R, FILE *Arquivo);
// bool CorrigeApontadores(No R, FILE *Arquivo, long position);
bool pesquisa(Registro *x, FILE *Arquivo);

int main()
{
    No R = { 0 };
    FILE *arquivoBinario;
    R.direita = -1;
    R.esquerda = -1;

    if ((arquivoBinario = fopen("arvoreBCem.bin", "rb")) == NULL)
    {
        printf("Erro na abertura do arquivo\n");
        exit(1);
    }

    // if ((arquivo = fopen("registrosDesordenadosCem.bin", "rb")) == NULL)
    // {
    //     printf("Erro na abertura do arquivo\n");
    //     exit(1);
    // }

    // while (fread(&R.dadoNo, sizeof(Registro), 1, arquivo))
    // {
    //     if (!InsereRegistro(R, arquivobinario))
    //     {
    //         printf("Falha ao inserir\n");
    //         continue;
    //     }
    // }

    printf("Digite a chave do registro que esta procurando: ");
    scanf("%u", &R.dadoNo.chave);

    if(pesquisa(&R.dadoNo, arquivoBinario)){
        printf("Chave: %u\n", R.dadoNo.chave);
        printf("Dado 1: %ld\n", R.dadoNo.dado1);
        printf("Dado 2: %s\n", R.dadoNo.dado2);
        printf("Dado 3: %s\n", R.dadoNo.dado3);
    } else {
        printf("Item nao encontrado\n");
    }

    // fclose(arquivo);
    fclose(arquivoBinario);
    return 0;
}

// bool InsereRegistro(No R, FILE *Arquivo)
// {
//     // Envia o ponteiro para o início do arquivo e armazena a posição inicial
//     fseek(Arquivo, 0, SEEK_SET);
//     long IniPosition = ftell(Arquivo);

//     // Envia o ponteiro para o local onde o novo dado será inserido e armazena a posição
//     fseek(Arquivo, 0, SEEK_END);
//     long position = ftell(Arquivo);

//     // Verifica se a posição inicial é a mesma que o item será inserido
//     if (position == IniPosition)
//     {
//         // Caso verdadeiro, isso significa que o arquivo está vazio, então podemos inserir direto
//         printf("Primeiro item inserido\n");
//         fwrite(&R, sizeof(No), 1, Arquivo);
//         return true;
//     }
//     else
//     {
//         // Mandamos o ponteiro do arquivo para a posição inicial
//         fseek(Arquivo, 0, SEEK_SET);
//         // Chamada da função para corrigir os apontadores
//         return CorrigeApontadores(R, Arquivo, position);
//     }
// }

bool pesquisa(Registro *x, FILE *Arquivo){

    No temp;
    long pos = 0;
    if(!read_node(pos, &temp)){
        //arvore vazia
        return false;
    }
    rewind(Arquivo);

    while (read_node(pos, &temp)){
        if(x->chave == temp.dadoNo.chave){
            *x = temp.dadoNo;
            return true;
        }
        else if (temp.dadoNo.chave > x->chave && temp.esquerda != -1){
            pos = temp.esquerda;
        }
        else if (temp.dadoNo.chave < x->chave && temp.direita != -1){
            pos = temp.direita;
        }
        else{
            return false;
        }
    }
    
    return false;

    // No temp;
    // fseek(Arquivo, position, SEEK_SET);
    // fread(&temp, sizeof(No), 1, Arquivo);
    // if(x->chave == temp.dadoNo.chave){
    //     *x = temp.dadoNo;
    //     return true;
    // } else if(x->chave > temp.dadoNo.chave){
    //     position = temp.direita;
    //     return pesquisa(x, Arquivo, position);
    // } else {
    //     position = temp.esquerda;
    //     return pesquisa(x, Arquivo, position);
    // }
}

// bool CorrigeApontadores(No R, FILE *Arquivo, long position)
// {
//     //variavel temporaria que ira ler os itens do arquivo para a comparacao
//     No temp;
//     //variavel que armazena a posicao do item a ser lido e leitura do item
//     long CurPosition = ftell(Arquivo);
//     fread(&temp, sizeof(No), 1, Arquivo);
//     //verificacao se a chave ja esta presente no arquivo
//     if (temp.dadoNo.chave == R.dadoNo.chave)
//     {
//         //caso verdadeiro retornamos que a chave ja esta presente e portanto nao foi inserida
//         printf("A chave já está presente no arquivo\n");
//         return false;
//     }
//     //verificando se a chave presente no arquivo e menor que a que queremos inserir
//     else if (temp.dadoNo.chave > R.dadoNo.chave)
//     {
//         //caso verdadeiro verificamos se o apontador a esquerda é nulo
//         if (temp.esquerda == -1)
//         {
//             //voltamos o ponteiro do arquivo para a posicao lida
//             fseek(Arquivo, CurPosition, SEEK_SET);
//             //atribuimos ao aposntador a esquerda a posicao onde sera inserido o novo item
//             temp.esquerda = position;
//             //sobrescrevemos a linha ja com o apontador atualizado
//             fwrite(&temp, sizeof(No), 1, Arquivo);
//             //mandamos o ponteiro para o final do arquivo e inserimos o novo item
//             fseek(Arquivo, 0, SEEK_END);
//             fwrite(&R, sizeof(No), 1, Arquivo);
//             return true;
//         }
//         else
//         {
//             //se o apontador nao for nulo, mandamos o ponteiro para o apontador a esquerda
//             fseek(Arquivo, temp.esquerda, SEEK_SET);
//             //chamamos a funcao recursivamente 
//             return CorrigeApontadores(R, Arquivo, position);
//         }
//     }
//     //verificando se a chave presente no arquivo e maior que a que queremos inserir
//     else if (temp.dadoNo.chave < R.dadoNo.chave)
//     {
//         //caso verdadeiro verificamos se o apontador a direita é nulo
//         if (temp.direita == -1)
//         {            
//             //voltamos o ponteiro do arquivo para a posicao lida
//             fseek(Arquivo, CurPosition, SEEK_SET);
//             //atribuimos ao aposntador a direita a posicao onde sera inserido o novo item
//             temp.direita = position;
//             //sobrescrevemos a linha ja com o apontador atualizado 
//             fwrite(&temp, sizeof(No), 1, Arquivo);
//             //mandamos o ponteiro para o final do arquivo e inserimos o novo item
//             fseek(Arquivo, 0, SEEK_END);
//             fwrite(&R, sizeof(No), 1, Arquivo);
//             return true;
//         }
//         else
//         {
//             //se o apontador nao for nulo, mandamos o ponteiro para o apontador a direita 
//             fseek(Arquivo, temp.direita, SEEK_SET);                     
//             //chamamos a funcao recursivamente 
//             return CorrigeApontadores(R, Arquivo, position);
//         }
//     }
    
//     return false;
// }
