#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
    FILE *arq;
    FILE *arqTextoGerado;
    Registro reg;
    if((arq = fopen("registrosDesordenadosCem.bin", "rb")) == NULL){
        perror("Erro na criação do arquivo\n");
        exit(1);
    }

    if((arqTextoGerado = fopen("registrosOrdenados.txt", "w")) == NULL){
        perror("Erro na criação do arquivo\n");
        exit(1);
    }

    while(fread(&reg, sizeof(reg), 1, arq) == 1){
        fprintf(arqTextoGerado, "%d\n", reg.chave);
        fprintf(arqTextoGerado, "%ld\n", reg.dado1);
        fprintf(arqTextoGerado, "%s\n\n", reg.dado2);
        fprintf(arqTextoGerado, "%s\n\n", reg.dado3);
    }

    fclose(arq);
    fclose(arqTextoGerado);
    return 0;

} 
