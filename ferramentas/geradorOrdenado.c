#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define SIZE_DADO2 1001
#define SIZE_DADO3 5001
#define QUANT_REG 100

typedef struct{
    unsigned int chave;
    long int dado1;
    char dado2[SIZE_DADO2];
    char dado3[SIZE_DADO3];
}Registro;

int main(int argc, char *argv[])
{

    srand(time(NULL));

    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    size_t charsetSize = sizeof(charset) - 1;

    Registro reg;
    FILE *arq;
    int chaveAtual = 0;
    if((arq = fopen("registrosOrdenadosCem.bin", "wb")) == NULL){
        perror("Erro na criação do arquivo\n");
        exit(1);
    }

    //Escrita dos registros em arquivo binario
    for(int i = 0; i < QUANT_REG; i++){
        //Obtencao da chave
        reg.chave = chaveAtual + rand() % (INT_MAX - chaveAtual + 1); 
        chaveAtual = reg.chave; //Garantir que as chaves serao crescentes

        //Obtencao do dado1
        reg.dado1 = (long)rand();

        //Obtencao do dado2
        for(int j = 0; j < SIZE_DADO2 - 1; j++){
            int indiceAleatorio = rand() % charsetSize;
            reg.dado2[j] = charset[indiceAleatorio];
        }

        reg.dado2[SIZE_DADO2 - 1] = '\0';

        //Obtencao do dado3
        for(int k = 0; k < SIZE_DADO3 - 1; k++){
            int indiceAleatorio = rand() % charsetSize;
            reg.dado3[k] = charset[indiceAleatorio];
        }

        reg.dado2[SIZE_DADO3 - 1] = '\0';
        
        //Escrita dos valores obtidos no arquivo binario
        fwrite(&reg, sizeof(reg), 1, arq);
    }

    fclose(arq);
    return 0;
}