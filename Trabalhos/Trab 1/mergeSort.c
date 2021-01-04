// Disciplina: Computacao Concorrente  
// Prof.: Silvana Rossetto  
// Módulo 1 - Trabalho 1  
// Código: Merge sort com paralelismo de dados  
// Feito por Rafael da Silva Fernandes

// Links de referência:
// https://pt.wikipedia.org/wiki/Merge_sort
// https://www.geeksforgeeks.org/merge-sort/
// https://www.youtube.com/watch?v=JSceec-wEyw&ab_channel=GeeksforGeeks


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

#define NTHREADS 4 // Número de threads
#define MAX 20 // Número de variáveis do vetor

// Variáveis globais
int vetor[MAX]; // Vetor de tamanho MAX
int bloco = 0; // Divisão em blocos para o paralelismo de dados
pthread_mutex_t mutex; // Variável de lock para exclusão mútua

// Funções utilizadas
void merge(int baixo, int meio, int alto); // Função merge para juntar duas partes
void merge_sort_auxiliar(int baixo, int alto); // Função auxiliar para o cálculo do merge sort
void* merge_sort_concorrente(void* arg); // Função merge sort executada pelas threads


// Fluxo principal
int main(void){

    // Gerando valores aleatórios para o vetor
    srand(time(NULL)); // Função que garante números diferentes para cada execução do programa

    printf("Nosso vetor original é:\n");
    for(int i = 0; i < MAX; i++){
        vetor[i] = rand() % 100;
        
        if(i == (MAX - 1)){
            printf("%d\n", vetor[i]);
        } else{
           printf("%d ", vetor[i]);
        }
    }
    
    // Variáveis para contagem de tempo do merge sort
    double tempoInicio, tempoFim, deltaTempo;
    GET_TIME(tempoInicio); // Início da contagem de tempo

    // Inicia o mutex (lock de exclusão mútua)
    pthread_mutex_init(&mutex, NULL);

    // Criação das threads
    pthread_t *tid; // Identificadores das threads no sistema
    tid = (pthread_t *) malloc(sizeof(pthread_t) * NTHREADS); // Alocação de memória para tid

    if(tid == NULL){
        fprintf(stderr,"ERRO -- malloc tid\n");
        return 1;
    }

    for(int i = 0; i < NTHREADS; i++){
        if(pthread_create((tid + i), NULL, merge_sort_concorrente, (void *) NULL)){
            fprintf(stderr, "ERRO -- pthread_create\n");
            return 2;
        }
    }

    // Aguarda o término das threads
    for(int i = 0; i < NTHREADS; i++){
        pthread_join(*(tid + i), NULL);
    }

    // Junção dos 4 blocos finais
    merge(0, (MAX/ 2 - 1)/ 2, MAX/ 2 - 1); 
    merge(MAX/ 2, MAX/ 2 + (MAX-1-MAX/ 2)/2, MAX - 1); 
    merge(0, (MAX - 1)/ 2, MAX - 1); 

    pthread_mutex_destroy(&mutex);

    GET_TIME(tempoFim); // Fim da contagem de tempo

    // Apresentação do vetor finalmente ordenado
    printf("Vetor ordenado:\n");
    for(int i = 0; i < MAX; i++){

        if(i == (MAX - 1)){
            printf("%d\n", vetor[i]);
        } else{
           printf("%d ", vetor[i]);
        }
    }

    deltaTempo = tempoFim - tempoInicio; // Cálculo do tempo decorrido
    printf("Tempo decorrido: %lf\n", deltaTempo);

    // Libera as áreas de memória alocadas
    free(tid);

    return 0;    
}

// Função merge para junção de dois blocos
// O primeiro subvetor é vetor[baixo...meio]
// O segundo subvetor é vetor[meio+1...alto]
void merge(int baixo, int medio, int alto){

    int ladoEsquerdo = medio - baixo + 1; // Tamanho do lado esquerdo
    int ladoDireito = alto - medio; // Tamanho do lado direito

    // Criação de vetores temporários
    int esquerdo[ladoEsquerdo];
    int direito[ladoDireito];

    // Guardando os valores no lado esquerdo
    for(int i = 0; i < ladoEsquerdo; i++){
        esquerdo[i] = vetor[i + baixo];
    }

    // Guardando os valores no lado direito
    for(int i = 0; i < ladoDireito; i++){
        direito[i] = vetor[i + medio + 1];
    }

    // Junção dos vetores temporários de volta à vetor[baixo...alto]
    int indexEsquerdo = 0; // Index inicial do primeiro subvetor
    int indexDireito = 0;  // Index inicial do segundo subvetor
    int indexMerged = baixo; // Index inicial do vetor junto

    // Merge lados esquerdo e direito em ordem ascendente
    while(indexEsquerdo < ladoEsquerdo && indexDireito < ladoDireito){
        if(esquerdo[indexEsquerdo] <= direito[indexDireito]){
            vetor[indexMerged++] = esquerdo[indexEsquerdo++];
        } else{
            vetor[indexMerged++] = direito[indexDireito++];
        }
    }

    // Inserção dos valores remanescentes da esquerda
    while(indexEsquerdo < ladoEsquerdo){
        vetor[indexMerged++] = esquerdo[indexEsquerdo++];
    }

    // Inserção dos valores remanscentes da direita
    while(indexDireito < ladoDireito){
        vetor[indexMerged++] = direito[indexDireito++];
    }
}

// Função auxiliar para o cálculo do merge sort
void merge_sort_auxiliar(int baixo, int alto){

    // Calculando o valor medio do vetor
    int medio = baixo + ((alto - baixo)/ 2);

    // Avaliando os parâmetros passados
    if(baixo < alto){
        merge_sort_auxiliar(baixo, medio); // Chamando a primeira metade do vetor
        merge_sort_auxiliar(medio + 1, alto); // Chamando a segunda metade do vetor
        merge(baixo, medio, alto); // Juntando as duas metades
    }
}

// Função executada pelas threads
void* merge_sort_concorrente(void* arg){

    // Entrada na seção crítica
    pthread_mutex_lock(&mutex);

    int thread_bloco = bloco++; // Qual bloco dos 4 totais

    // Saída da seção crítica
    pthread_mutex_unlock(&mutex);

    // Cálculo dos valores baixo, medio e alto
    int baixo = thread_bloco * (MAX/ NTHREADS);
    int alto = (thread_bloco + 1) * (MAX/ NTHREADS) - 1;
    int medio = baixo + ((alto - baixo)/ 2);

    // Avaliando os valores obtidos
    if(baixo < alto){
        merge_sort_auxiliar(baixo, medio); // Chamando a função merge_sort_auxiliar para a primeira metade
        merge_sort_auxiliar(medio + 1, alto); // Chamando a função merge_sort_auxiliar segunda metade
        merge(baixo, medio, alto); // Juntando as duas metades
    }

    pthread_exit(NULL);
}