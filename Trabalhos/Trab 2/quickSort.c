// Disciplina: Computacao Concorrente  
// Prof.: Silvana Rossetto  
// Módulo 2 - Trabalho 2  
// Código: Quick sort concorrente   
// Feito por Rafael da Silva Fernandes

/*
Links de referência:
https://www.youtube.com/watch?v=es2T6KY45cA&ab_channel=udiprod
https://pt.wikipedia.org/wiki/Quicksort
https://www.ime.usp.br/~pf/algoritmos/aulas/quick.html
https://software.intel.com/content/www/us/en/develop/articles/an-efficient-parallel-three-way-quicksort-using-intel-c-compiler-and-openmp-45-library.html
https://www.cs.cmu.edu/afs/cs/academic/class/15213-s13/www/lectures/26-parallelism.pdf
https://blog.pantuza.com/artigos/o-algoritmo-de-ordenacao-quicksort
https://stackoverflow.com/questions/21114591/c-programming-read-specific-line-from-text-file
https://stackoverflow.com/questions/36025219/reading-integers-from-a-buffer-into-an-array-in-c
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "timer.h"

#define NTHREADS 31 // número total de threads
#define TAM 30 // tamanho do vetor a ser ordenado

// Variáveis globais
int vetor[TAM]; // vetor a ser ordenado


// Estruturas de dados


// Funções utilizadas
void troca(int esquerda, int direita); // faz a troca entre duas podições no vetor
int particionamento(int inicio, int fim); // retorna a posição do pivô no vetor
void quickSortSequencial(); // função quick sort sequencial
void* quickSortConcorrente(void *arg); // função quick sort executada pelas threads


// Fluxo principal 
int main(void){

    // Importanto numeros.txt e colocando-o no vetor
    FILE *numeros = fopen("numeros.txt", "r");
    if(numeros == NULL){
        printf("---ERRO quickSort.c: problema ao ler numeros.txt\n");
        exit(-1);
    } else{
        char linha[256];

        // Pega primeira linha do arquivo numeros.txt
        fgets(linha, sizeof(linha), numeros);
        int numeroTotalLinhas = atoi(linha); // número total de linhas do arquivo
        printf("Número total de linhas com número para serem ordenados é: %d\n", numeroTotalLinhas - 1);
        
        int numeroLinha; // número da linha desejada (inicia em 1)
        printf("Entre com o valor da linha desejada: ");
        scanf("%d", &numeroLinha);

        // Lê linha desejada
        printf("Linha desejada: %d\n\n", numeroLinha);
        for(int i = 1; i < numeroTotalLinhas; i++){
            fgets(linha, sizeof(linha), numeros);
            printf("Números da linha %d = %s\n", i, linha);
            
            if(i == numeroLinha){
                for(int j = 0; j < TAM; j++){
                    // ToDo: sscanf está alterando o vetor da linha desejada. Por que?
                    sscanf(&linha[j], "%d", &vetor[j]);
                }
            }    
        }

        fclose(numeros);
    }    

    // Imprimindo vetor original
    printf("\nVetor original:\n");
    for(int i = 0; i < TAM; i++){
        if(i == (TAM - 1)){
            printf("%d\n", vetor[i]);
        } else{
            printf("%d ", vetor[i]);
        }
    }

    // Testando código
    quickSortSequencial(0, TAM-1);

    // Imprimindo vetor ordenado
    printf("\nVetor ordenado:\n");
    for(int i = 0; i < TAM; i++){
        if(i == (TAM -1)){
            printf("%d\n", vetor[i]);
        } else{
            printf("%d ", vetor[i]);
        }
    }

    return 0;
}

// Método para fazer a troca de dados entre as duas posições no vetor
void troca(int esquerda, int direita){
    int auxiliar = vetor[esquerda];
    vetor[esquerda] = vetor[direita];
    vetor[direita] = auxiliar;
}

// Particionamento
// Retorna a posição do pivô no vetor
int particionamento(int inicio, int fim){
    int posicaoPivo = inicio; // posição do pivô no vetor

    // Eleemento atual é menor igual ao pivõ?
    for(int i = inicio; i < fim; i++){
        if(vetor[i] <= vetor[fim]){
            troca(posicaoPivo++, i);
        }
    }

    troca(posicaoPivo, fim);

    return posicaoPivo;
}

// Quick sort sequencial
// Utilizado caso o número de threads seja maior ou igual o número de elementos do vetor
void quickSortSequencial(int inicio, int fim){
    if(inicio < fim){
        int pivo = particionamento(inicio, fim);
        quickSortSequencial(inicio, pivo - 1);
        quickSortSequencial(pivo + 1, fim);
    }
}