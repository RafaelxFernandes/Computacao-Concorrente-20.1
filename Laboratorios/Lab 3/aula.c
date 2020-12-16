/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Semana 3 */
/* Código: Cálculo de uma série de valores inteiros */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

/* Variáveis globais */
long int dimensao; /* Dimensão do valor de entrada */
int nThreads; /* Número de threads */
double *vetor; /* Vetor de entrada com dimensão igual à dimensao */


/* Fluxo das threads */
void *soma(void *arg){

    long int id = (long int) arg; /* Identificador da thread */
    long int tamanhoBloco = dimensao/ nThreads; /* Tamanho do bloco de cada thread */    
    long int posicaoInicio = id * tamanhoBloco; /* Elemento inicial do bloco da thread */
    long int posicaoFim; /* Elemento final (não processado) do bloco */
    double *somaLocal;

    somaLocal = (double *) malloc(sizeof(double));

    if(somaLocal == NULL){
        fprintf(stderr, "ERRO -- malloc somaLocal\n");
        exit(1);
    }

    *somaLocal = 0; 

    /* Tratamento do resto (se houver) */
    if(id == (nThreads-1)){
        posicaoFim = dimensao;
    } else{
        posicaoFim = posicaoInicio + tamanhoBloco;
    }

    for(long int i = posicaoInicio; i < posicaoFim; i++){
        *somaLocal += vetor[i];
    }

    pthread_exit((void *) somaLocal);
}

/* Fluxo principal */
int main(int argc, char *argv[]){

    double somaSequencial = 0;
    double somaConcorrente = 0;
    double tempoInicio, tempoFim; /* Variáveis para contagem de tempo */
    pthread_t *tid; /* Identificadores das threads no sistema */
    double *retorno; /* Valor do retorno das threads */

    /* Recebe e valida os parâmetros de entrada (dimensão do vetor, número de threads) */
    if(argc < 3){
        fprintf(stderr, "Digite: %s <dimensão do vetor> <número de threads>\n", argv[0]);
        return 1;
    }

    dimensao = atoi(argv[1]);
    nThreads = atoi(argv[2]);

    /* Aloca o vetor de entrada */
    vetor = (double *) malloc(sizeof(double) * dimensao);

    if(vetor == NULL){
        fprintf(stderr, "ERRO -- malloc vetor\n");
        return 2;
    }

    /* Preenche o vetor de entrada */
    for(long int i = 0; i < dimensao; i++){
        vetor[i] = 1000.1/ (i + 1);
    }

    /* Soma sequencial dos elementos */
    GET_TIME(tempoInicio);
    
    for(long int i = 0; i < dimensao; i++){
        somaSequencial += vetor[i];
    }

    GET_TIME(tempoFim);
    printf("Tempo sequencial: %lf\n", tempoFim - tempoInicio);

    /* Soma concorrente dos elementos */
    GET_TIME(tempoInicio);
    tid = (pthread_t *) malloc(sizeof(pthread_t) * nThreads);

    if(tid == NULL){
        fprintf(stderr, "ERRO -- malloc tid\n");
        return 2;
    }

    /* Criar as threads */
    for(long int i = 0; i < nThreads; i++){
        if(pthread_create((tid + i), NULL, soma, (void *) i)){
            fprintf(stderr, "ERRO -- pthread_create\n");
            return 3;
        }
    }

    /* Aguardar o término das threads */
    for(long int i = 0; i < nThreads; i++){
        pthread_join(*(tid + i), (void **) &retorno);    
        somaConcorrente += *retorno;
        free(retorno);
    }

    GET_TIME(tempoFim);
    printf("Tempo concorrente: %lf\n", tempoFim - tempoInicio);

    /* Exibir os resultados */
    printf("Soma sequencial: %.15lf\n", somaSequencial);
    printf("Soma concorrente: %.15lf\n", somaConcorrente);

    /* Libera as áreas de memória alocadas */
    free(vetor);
    free(tid);

    return 0;
}