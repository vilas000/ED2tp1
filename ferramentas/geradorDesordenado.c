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

void embaralha(Registro *vetor, size_t n){

    if (n > 1) {
        for (size_t i = 0; i < n - 1; i++) {
            size_t j = i + rand() % (n - i);
            Registro temp = vetor[i];
            vetor[i] = vetor[j];
            vetor[j] = temp;
        }
    }
}

int main(int argc, char *argv[])
{

    srand(time(NULL));

    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    size_t charsetSize = sizeof(charset) - 1;

    Registro reg[QUANT_REG];
    FILE *arq;

    int chaveAtual = 0;
    if((arq = fopen("registrosDesordenadosCem.bin", "wb")) == NULL){
        perror("Erro na criação do arquivo\n");
        exit(1);
    }

    //Escrita dos registros em arquivo binario
    for(int i = 0; i < QUANT_REG; i++){

        //Obtencao da chave
        reg[i].chave = chaveAtual + rand() % (INT_MAX - chaveAtual + 1); 
        chaveAtual = reg[i].chave; //Garantir que as chaves serao crescentes

        //Obtencao do dado1
        reg[i].dado1 = (long)rand();

        //Obtencao do dado2
        for(int j = 0; j < SIZE_DADO2 - 1; j++){
            int indiceAleatorio = rand() % charsetSize;
            reg[i].dado2[j] = charset[indiceAleatorio];
        }

        reg[i].dado2[SIZE_DADO2 - 1] = '\0';

        //Obtencao do dado3
        for(int k = 0; k < SIZE_DADO3 - 1; k++){
            int indiceAleatorio = rand() % charsetSize;
            reg[i].dado3[k] = charset[indiceAleatorio];
        }

        reg[i].dado3[SIZE_DADO3 - 1] = '\0';

    }

    embaralha(reg, QUANT_REG);

    //Escrita dos valores obtidos no arquivo binario
    for(int i = 0; i < QUANT_REG; i++){
        if (fwrite(&reg[i], sizeof(Registro), 1, arq) != 1) {
            perror("Erro ao escrever no arquivo");
            fclose(arq);
            return 1;
        }
    }
    
    fclose(arq);
    return 0;
}