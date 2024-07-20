#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE_DADO2 1001
#define SIZE_DADO3 5001
#define QUANT_REG 100
#define M 4
#define MM (M * 2)

//1373337

typedef struct{
    unsigned int chave;
    long int dado1;
    char dado2[SIZE_DADO2];
    char dado3[SIZE_DADO3];
}Registro;

typedef struct TipoPaginaBin{
    short n;
    Registro registros[MM];
    int apontadores[MM + 1];
}TipoPaginaBin;

int main(int argc, char *argv[])
{
    FILE *arq;
    Registro reg;
    if((arq = fopen("registrosOrdenadosCem.bin", "rb")) == NULL){
        perror("Erro na criação do arquivo\n");
        exit(1);
    }

    while(fread(&reg, sizeof(reg), 1, arq) == 1){
        printf("%u\n", reg.chave);
        printf("%ld\n", reg.dado1);
        printf("%s\n\n", reg.dado2);
        printf("%s\n\n", reg.dado3);
    }

    fclose(arq);
    return 0;

} 