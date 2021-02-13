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

#define NTHREADS 4 // número total de threads
#define MAX 60 // tamanho máximo do vetor a ser ordenado

// Variáveis globais
int vetor[MAX]; // vetor a ser ordenado
int numeroElementos; // número de elementos do vetor
sem_t semaforoBinario; // semáforo binário

// Funções utilizadas
void troca(int esquerda, int direita); // faz a troca entre duas podições no vetor
int particionamento(int inicio, int fim); // retorna a posição do pivô no vetor
void quickSortSequencial(); // função quick sort sequencial
void* quickSortConcorrente(void *arg); // função quick sort executada pelas threads


// Fluxo principal 
int main(void){

    // Variáveis para contagem de tempo
    double tempoInicio, tempoFim, deltaTempo;
    GET_TIME(tempoInicio); // início da contagem de tempo

    // Importanto os arquivos .txt
    FILE *vetor1 = fopen("vetor1.txt", "r");
    FILE *vetor2 = fopen("vetor2.txt", "r");
    FILE *vetor3 = fopen("vetor3.txt", "r");
    FILE *vetor4 = fopen("vetor4.txt", "r");
    FILE *vetor5 = fopen("vetor5.txt", "r");
    
    // >>> Necessário alterar primeiro parâmetro para testar outro arquivo <<<
    fscanf(vetor1, "%d", &vetor[0]); 
    numeroElementos = vetor[0];

    pthread_t tid[NTHREADS];
    int threadID, *id;

    // Inicializa semáforo
    sem_init(&semaforoBinario, 0, 1); 

    // Tratamento de erros
    if((vetor1 == NULL) || (vetor2 == NULL) || (vetor3 == NULL) || (vetor4 == NULL) || (vetor5 == NULL)){
        printf("---ERRO quickSort.c: problema ao ler arquivo .txt\n");
        exit(-1);
    } else{ // Executa código

        printf("Número total de threads = %d\n", NTHREADS);

        // Primeiro elemento do vetor é o total de elementos a serem ordenados
        printf("Quantidade total de elementos do vetor selecionado = %d\n", numeroElementos);
        
        for(int j = 0; j < numeroElementos; j++){
            // >>> Necessário alterar primeiro parâmetro para testar outro arquivo <<<
            fscanf(vetor1, "%d", &vetor[j]); 
        }

        // Imprimindo vetor original
        printf("\nVetor original:\n");
        for(int i = 0; i < numeroElementos; i++){
            if(i == (numeroElementos - 1)){
                printf("%d\n\n", vetor[i]);
            } else{
                printf("%d ", vetor[i]);
            }
        }

        // Execução quick sort
        if(numeroElementos <= NTHREADS){ // executa versão sequencial
            quickSortSequencial(0, numeroElementos - 1);
        } else{ // executa versão concorrente

            for(threadID = 0; threadID < NTHREADS; threadID++){
                // Aloca espaço para o identificador da thread
                if((id = malloc(sizeof(int))) == NULL){
                    pthread_exit(NULL);
                    return 1;
                }

                *id = threadID;

                // Criação das threads
                if(pthread_create(&tid[threadID], NULL, quickSortConcorrente, (void *) id)){
                    printf("---ERRO pthread_create\n");
                    return 2;
                }
            }

            // Aguarda o término das threads
            for(int i = 0; i < NTHREADS; i++){
                pthread_join(*(tid + i), NULL);
            }

            free(id);
        }

        GET_TIME(tempoFim); // fim da contagem de tempo

        // Imprimindo vetor ordenado
        printf("\nVetor ordenado:\n");
        for(int i = 0; i < numeroElementos; i++){
            if(i == (numeroElementos -1)){
                printf("%d\n", vetor[i]);
            } else{
                printf("%d ", vetor[i]);
            }
        }

        // Cálculo do tempo decorrido
        deltaTempo = tempoFim - tempoInicio;
        printf("\nTempo decorrido = %lf\n", deltaTempo);

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

    // Elemento atual é menor ou igual ao pivõ?
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

// Quick sort concorrente
// Utilizando caso o número de threads seja menor do que o número de elementos do vetor
void* quickSortConcorrente(void *arg){
    
    int *threadID = (int*) arg; // identificador da thread
    printf("Thread %d está executando...\n", *threadID);

    // Início seção crítica
    sem_wait(&semaforoBinario);

    int pivo = particionamento(0, (numeroElementos - 1));

    if(pivo > 1){
        quickSortSequencial(0, pivo);
    } if((numeroElementos - 1) > (pivo + 1)){
        quickSortSequencial((pivo + 1), (numeroElementos - pivo - 1));
    }

    // Fim seção crítica
    sem_post(&semaforoBinario);

    printf("Thread %d terminou\n", *threadID);
    free(arg);
    pthread_exit(NULL);
}
