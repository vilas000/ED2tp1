#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#define SIZE_DADO2 1001
#define SIZE_DADO3 5001
#define TAM_PAG 5

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
    struct No *nos[TAM_PAG];
    long nNos;
    struct Pagina *proxima;
} Pagina;

bool InsereRegistro(No R, FILE *Arquivo, long int qtt);
Pagina* criarPagina();

int main()
{
    No R = { 0 };
    FILE *arquivo;
    FILE *arquivoBinario;
    R.direita = -1;
    R.esquerda = -1;

    long int qtt = 0;

    if ((arquivoBinario = fopen("arvoreBinaria.bin", "w+b")) == NULL)
    {
        printf("Erro na abertura do arquivo binrio\n");
        exit(1);
    }
    if ((arquivo = fopen("registrosDesordenadosCem.bin", "rb")) == NULL)
    {
        printf("Erro na abertura do arquivo\n");
        fclose(arquivoBinario);
        exit(1);
    }


    while (fread(&R.dadoNo, sizeof(Registro), 1, arquivo) > 0)
    {
        if (!InsereRegistro(R, arquivoBinario, qtt))
        {
            printf("Falha ao inserir\n");
            continue;
        }
        qtt++;

        No temp;
        rewind(arquivoBinario);
        while (fread(&temp, sizeof(No), 1, arquivoBinario)) {
            printf("[%ld]: <%ld, %u, %ld>\n", qtt,
                temp.esquerda, temp.dadoNo.chave, temp.direita);
        }
    }

    fclose(arquivo);
    fclose(arquivoBinario);
    return 0;
}

Pagina* criarPagina(){
    Pagina* temp = (Pagina*)malloc(sizeof(Pagina));
    temp->nNos = 0;
    temp->proxima = NULL;
    return temp;
}

bool InsereRegistro(No R, FILE * Arquivo, const long qtt) {
    #define read_node(i, noLido)    (!fseek(Arquivo, i * sizeof(No), SEEK_SET)) && (fread(noLido, sizeof(No), 1, Arquivo) > 0)
    //#define write_node(i, noEscrito)    (!fseek(Arquivo, i * sizeof(No), SEEK_SET)) && (fwrite(noEscrito, sizeof(No), 1, Arquivo) > 0)
    
    printf("[%s] (qtt:%ld)\n", __func__, qtt);

    if (qtt == 0){
        // Caso verdadeiro, isso significa que o arquivo está vazio, então podemos inserir direto
            fwrite(&R, sizeof(No), 1, Arquivo);
            printf("Primeiro item inserido\n");
            // printf("%ld %ld\n", R.esquerda, R.direita);
            return true;

        }

    No temp;
    long pos = 0;

    while (read_node(pos, &temp))
    {
        printf("[%s] qtt: %ld, pos=%ld, temp: <%ld, %d, %ld>\n", __func__, qtt, pos, temp.esquerda, temp.dadoNo.chave, temp.direita);
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
            printf("%ld %ld\n", temp.esquerda, temp.direita);

    return false;
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