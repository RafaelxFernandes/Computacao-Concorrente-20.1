// Disciplina: Computacao Concorrente  
// Prof.: Silvana Rossetto  
// Módulo 2 - Trabalho 2  
// Código: Programa auxiliar para casos de teste
// Feito por Rafael da Silva Fernandes

#include <stdio.h>
#include <stdlib.h>

#define N 6 // número total de linhas do arquivo
#define MAX 60

int main(void){
    FILE *file = fopen("numeros.txt", "w");

    if(file == NULL){
        printf("---ERRO rand.c: não conseguiu abrir o arquivo numeros.txt");
        exit(-1);
    }

    fprintf(file, "%d\n", N);

    // Gerando 2 linhas de 30 elementos com números aleatórios
    for(int i = 0; i < MAX; i++){
        int numeroAleatorio = rand() % 100;

        if(i == (MAX/2 - 1)){
            fprintf(file, "%d\n", numeroAleatorio);
        } else{
           fprintf(file, "%d ", numeroAleatorio);
        }
    }

    // Gerando linha com números já ordenados
    fprintf(file, "\n");
    for(int i = 0; i < MAX/2; i++){
        if(i == (MAX/2 - 1)){
            fprintf(file, "%d\n", i);
        } else{
            fprintf(file, "%d ", i);
        }
    }

    // Gerando linha com números ordenados de forma decrescente
    for(int i = MAX/2; i > 0; i--){
        if(i == (1)){
            fprintf(file, "%d\n", i);
        } else{
            fprintf(file, "%d ", i);
        }
    }

    // Gerando linha com números iguais
    for(int i = 0; i < MAX/2; i++){
        if(i == (MAX/2 - 1)){
            fprintf(file, "%d\n", 1);
        } else{
            fprintf(file, "%d ", 1);
        }
    }

    fclose(file);

    return 0;
}