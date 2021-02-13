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

    FILE *vetor1 = fopen("vetor1.txt", "w"); // arquivo com números aleatórios
    FILE *vetor2 = fopen("vetor2.txt", "w"); // arquivo com números aleatórios
    FILE *vetor3 = fopen("vetor3.txt", "w"); // arquivo com números já ordenados
    FILE *vetor4 = fopen("vetor4.txt", "w"); // arquivo com números ordenados decrescentemente
    FILE *vetor5 = fopen("vetor5.txt", "w"); // arquivo com números iguais
    // Observação: o primeiro número em cada arquivo é a quantidade de elementos no mesmo

    // Tratamento de erros
    if(vetor1 == NULL){
        printf("---ERRO rand.c: não conseguiu abrir o arquivo vetor1.txt");
        exit(-1);
    } if(vetor2 == NULL){
        printf("---ERRO rand.c: não conseguiu abrir o arquivo vetor2.txt");
        exit(-1);
    } if(vetor3 == NULL){
        printf("---ERRO rand.c: não conseguiu abrir o arquivo vetor3.txt");
        exit(-1);
    } if(vetor4 == NULL){
        printf("---ERRO rand.c: não conseguiu abrir o arquivo vetor4.txt");
        exit(-1);
    } if(vetor5 == NULL){
        printf("---ERRO rand.c: não conseguiu abrir o arquivo vetor5.txt");
        exit(-1);
    }

    // Gerando 2 vetores de 30 elementos com números aleatórios
    fprintf(vetor1, "%d ", MAX/2);
    fprintf(vetor2, "%d ", MAX/2);

    for(int i = 0; i < MAX/2; i++){

        int numeroAleatorio1 = rand() % 100;
        int numeroAleatorio2 = rand() % 200;

        if(i == (MAX/2 - 1)){
            fprintf(vetor1, "%d\n", numeroAleatorio1);
            fprintf(vetor2, "%d\n", numeroAleatorio2);
        } else{
            fprintf(vetor1, "%d ", numeroAleatorio1);
            fprintf(vetor2, "%d ", numeroAleatorio2);
        }
    }

    // Gerando linha com números já ordenados
    fprintf(vetor3, "%d ", MAX);

    for(int i = 0; i < MAX; i++){
        if(i == (MAX - 1)){
            fprintf(vetor3, "%d\n", i);
        } else{
            fprintf(vetor3, "%d ", i);
        }
    }

    // Gerando linha com números ordenados de forma decrescente
    fprintf(vetor4, "%d ", MAX/3);

    for(int i = MAX/3; i > 0; i--){
        if(i == 1){
            fprintf(vetor4, "%d\n", i);
        } else{
            fprintf(vetor4, "%d ", i);
        }
    }

    // Gerando linha com números iguais
    fprintf(vetor5, "%d ", MAX/4);

    for(int i = 0; i < MAX/4; i++){
        if(i == (MAX/4 - 1)){
            fprintf(vetor5, "%d\n", 1);
        } else{
            fprintf(vetor5, "%d ", 1);
        }
    }

    fclose(vetor1);
    fclose(vetor2);
    fclose(vetor3);
    fclose(vetor4);
    fclose(vetor5);

    return 0;
}