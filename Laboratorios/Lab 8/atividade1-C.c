/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 3 - Laboratório: 8, Atividade 1, letra C */
/* Código: leitores e escritores com prioridade para escrita, utilizando semáforos */
/* Feito por Rafael da Silva Fernandes */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NTHREADS 4 // número total de threads
#define TAM 10 // tamanho do vetor a ser lido/ sobrescrito

// Variáveis globais
sem_t em_e, em_l, escr, leit; //semaforos
int e=0, l=0; //globais
int vetor[TAM];

// Função executada pelas threads leitoras
void *leitor(void *arg){

    int *id = (int *) arg; 

    while(1){

        // Início seção crítica
        sem_wait(&leit);
        sem_wait(&em_l); l++;

        printf("Leitor %d quer ler\n", *id);
        
        // Primeiro leitor
        if(l==1) {
            sem_wait(&escr);
        }
        
        // Fim seção crítica
        sem_post(&em_l);
        sem_post(&leit);
        
        // Faz a leitura
        printf("Leitor %d está lendo...\n", *id);
        printf("Vetor atual:\n");
        for(int i = 0; i < TAM; i++){
            if(i == (TAM - 1)){
                printf("%d\n\n", vetor[i]);
            } else{
                printf("%d ", vetor[i]);
            }
        }

        printf("Leitor %d terminou de ler\n", *id);
        
        // Início seção crítica
        sem_wait(&em_l); l--;
        
        // Último leitor
        if(l==0) {
            sem_post(&escr);
            if(e > 0){
                printf("Um escritor foi desbloqueado!\n");
            }
        }
        
        // Fim seção crítica
        sem_post(&em_l);
        sleep(1);
    }

    pthread_exit(NULL);
}

// Função executada pelas threads escritoras
void *escritor(void *arg){

    int *id = (int *) arg; 

    while(1){

        // Início seção crítica
        sem_wait(&em_e); e++;
        printf("Escritor %d quer escrever\n", *id);

        // Primeiro escritor
        if(e==1){
            sem_wait(&leit);
        } 

        // Fim seção crítica
        sem_post(&em_e);
        sem_wait(&escr);

        // Modifica o vetor
        printf("Escritor %d está escrevendo...\n", *id);
        for(int i = 0; i < TAM; i++){
            if((i == 0) || (i == TAM-1)){
                vetor[i] = *id;
            } else{
                vetor[i] = *id + 10;
            }
        }

        printf("Escritor %d terminou de escrever\n", *id);

        // Início seção crítica
        sem_post(&escr);
        sem_wait(&em_e); e--;
        
        // Último escritor
        if(e==0) {
            sem_post(&leit);
            if(l > 0){
                printf("Um leitor foi desbloqueado!\n");
            }
        }

        // Fim seção crítica
        sem_post(&em_e);
        sleep(1); 
    }

    pthread_exit(NULL);
}

// Fluxo principal
int main(void){

    // Inicializa semáforos binários
    sem_init(&em_e, 0, 1);
    sem_init(&em_l, 0, 1);
    sem_init(&escr, 0, 1);
    sem_init(&leit, 0, 1);

    // Identificadores das threads
    pthread_t tid[NTHREADS];
    int t, *id;

    // Inicializa o vetor com 1's
    for(int i = 0; i < TAM; i++){
        vetor[i] = 1;
    }

    // Imprime vetor
    printf("Vetor original:\n");
    for(int i = 0; i < TAM; i++){
        if(i == (TAM - 1)){
            printf("%d\n\n", vetor[i]);
        } else{
            printf("%d ", vetor[i]);
        }
    }

    // Aloca espaço para o identificador da thread
    for(t = 0; t < NTHREADS; t++){
        if((id = malloc(sizeof(int))) == NULL){
            pthread_exit(NULL);
            return 1;
        } 
        
        *id = t;

        if(pthread_create(&tid[t], NULL, leitor, (void *)id)){
            printf("-- ERRO: pthread_create leitora\n");
            exit(-1);
        } if(pthread_create(&tid[t], NULL, escritor, (void *)id)){
            printf("-- ERRO: pthread_create escritora\n");
            exit(-1);
        }
    }

    // Espera todas as threads terminarem
    for(t = 0; t < NTHREADS; t++){
        if(pthread_join(tid[t], NULL)){
            printf("-- ERRO: pthread_join\n");
            exit(-1);
        }
    }

    pthread_exit(NULL);
}