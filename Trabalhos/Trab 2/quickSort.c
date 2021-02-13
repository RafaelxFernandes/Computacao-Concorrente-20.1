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
#define TAM 60 // tamanho máximo do vetor a ser ordenado

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

    // Importanto os arquivos .txt
    FILE *vetor1 = fopen("vetor1.txt", "r");
    FILE *vetor2 = fopen("vetor2.txt", "r");
    FILE *vetor3 = fopen("vetor3.txt", "r");
    FILE *vetor4 = fopen("vetor4.txt", "r");
    FILE *vetor5 = fopen("vetor5.txt", "r");

    // Tratamento de erros
    if((vetor1 == NULL) || (vetor2 == NULL) || (vetor3 == NULL) || (vetor4 == NULL) || (vetor5 == NULL)){
        printf("---ERRO quickSort.c: problema ao ler arquivo .txt\n");
        exit(-1);
    } else{
        // Primeiro elemento do vetor é o total de elementos a serem ordenados
        // Necessário alterar primeiro parâmetro para testar outro arquivo
        fscanf(vetor4, "%d", &vetor[0]); 
        int numeroElementos = vetor[0];
        printf("Quantidade total de elementos do vetor selecionado = %d\n", numeroElementos);
        
        for(int j = 0; j < numeroElementos; j++){
            // Necessário alterar primeiro parâmetro para testar outro arquivo
            fscanf(vetor4, "%d", &vetor[j]); 
        }

        // Imprimindo vetor original
        printf("\nVetor original:\n");
        for(int i = 0; i < numeroElementos; i++){
            if(i == (numeroElementos - 1)){
                printf("%d\n", vetor[i]);
            } else{
                printf("%d ", vetor[i]);
            }
        }

        // Testando código
        quickSortSequencial(0, numeroElementos-1);

        // Imprimindo vetor ordenado
        printf("\nVetor ordenado:\n");
        for(int i = 0; i < numeroElementos; i++){
            if(i == (numeroElementos -1)){
                printf("%d\n", vetor[i]);
            } else{
                printf("%d ", vetor[i]);
            }
        }

        fclose(vetor1);
        fclose(vetor2);
        fclose(vetor3);
        fclose(vetor4);
        fclose(vetor5);

        return 0;
    }    
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