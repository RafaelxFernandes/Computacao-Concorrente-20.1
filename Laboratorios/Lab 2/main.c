/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 2, Atividade */
/* Código: Multiplicação de matrizes quadradas */
/* Feito por Rafael da Silva Fernandes */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

/* Variáveis globais */
float *matriz1; /* Primeira matriz de entrada */
float *matriz2; /* Segunda matriz de entrada */
float *saida; /* Matriz de saída */
int nThreads; /* Número total de threads a serem criadas */

/* Funções utilizadas */
void *multiplica(void *arg);

/* Estruturas */
typedef struct{
    int id; /* Identificador do elemento que a thread irá processar */
    int dimensao; /* Dimensão das estruturas de entrada */
} tArgs;


/* Fluxo principal */
int main(int argc, char* argv[]){

    int dimensao; /* Dimensão das matrizes de entrada */
    pthread_t *tid; /* Identificadores das threads no sistema */
    tArgs *args; /* Identificadores locais das threads e dimensão */

    /* Variáveis para contagem de tempo de trechos do programa */
    double tempoInicio, tempoFim, deltaTempo;

    /* Início da contagem de tempo de inicialização */
    GET_TIME(tempoInicio);

    /* Leitura e avaliação dos parâmetros de entrada */
    if(argc < 3){
        printf("Digite: %s <número de threads> <dimensão da matriz>\n", argv[0]);
        return 1; /* Tratamento de erro */
    }

    nThreads = atoi(argv[1]);
    dimensao = atoi(argv[2]);

    if(nThreads > dimensao){
        nThreads = dimensao;
        printf("O número de threads desejado é maior do que a dimensão das matrizes.\n");
        printf("Por essa razão, ele será considerado IGUAL à dimensão das matrizes.\n\n");
    }

    /* Alocação de memória para as estruturas de dados */
    /* Matriz 1 - Primeira matriz de entrada */
    matriz1 = (float *) malloc(sizeof(float) * dimensao * dimensao);
    if(matriz1 == NULL){
        printf("ERRO -- malloc matriz1\n");
        return 2;
    }

    /* Matriz 2 - Segunda matriz de entrada */
    matriz2 = (float *) malloc(sizeof(float) * dimensao * dimensao);
    if(matriz2 == NULL){
        printf("ERRO -- malloc matriz2\n");
        return 2;
    }

    /* Matriz des saída */
    saida = (float *) malloc(sizeof(float) * dimensao * dimensao);
    if(saida == NULL){
        printf("ERRO -- malloc saida\n");
        return 2;
    }

    /* Inicialização das estruturas de dados */
    for(int i = 0; i < dimensao; i++){
        for(int j = 0; j < dimensao; j++){
            matriz1[i*dimensao + j] = 1; /* Equivalente à matriz1[i][j]; dimensao é referente às colunas*/
            matriz2[i*dimensao + j] = 1;
            saida[i*dimensao + j] = 0;
        }
    }

    /* Fim da contagem de tempo da inicialização */
    GET_TIME(tempoFim);
    deltaTempo = tempoFim - tempoInicio;
    printf("\nTempo de inicialização: %lf\n", deltaTempo);

    /* Multiplicação das matrizes */
    /* Início contagem de tempo da multiplicação de matrizes */
    GET_TIME(tempoInicio);

    /* Alocação das estruturas */
    tid = (pthread_t *) malloc(sizeof(pthread_t) * nThreads);
    if(tid == NULL){
        printf("ERRO -- malloc tid\n");
        return 2;
    }

    args = (tArgs *) malloc(sizeof(tArgs) * nThreads);
    if(args == NULL){
        printf("ERRO -- malloc args\n");
        return 2;
    }

    /* Criação das threads */
    for(int i = 0; i < nThreads; i++){
        (args + i)->id = i;
        (args + i)->dimensao = dimensao;

        if(pthread_create(tid + i, NULL, multiplica, (void *) (args+i))){
            printf("ERRO -- pthread_create\n");
            return 3;
        }
    }

    /* Espera pelo término das threads */
    for(int i = 0; i < nThreads; i++){
        pthread_join(*(tid + i), NULL);
    }

    /* Fim da contagem de tempo da multiplicação de matrizes */
    GET_TIME(tempoFim);
    deltaTempo = tempoFim - tempoInicio;
    printf("Tempo de multiplicação de matrizes: %lf\n", deltaTempo);

    /* Exibição das estruturas de dados.
    Está comentada para facilitar a visualização dos resultados esperados pelo exercício */
    /* Matriz 1 */
    /* printf("\nMatriz 1 de entrada:\n");
    for(int i = 0; i < dimensao; i++){
        for(int j = 0; j < dimensao; j++){
            printf("%.1f ", matriz1[i*dimensao + j]);
        }
        printf("\n");
    } */

    /* Matriz 2 */
    /* printf("\nMatriz 2 de entrada:\n");
    for(int i = 0; i < dimensao; i++){
        for(int j = 0; j < dimensao; j++){
            printf("%.1f ", matriz2[i*dimensao + j]);
        }
        printf("\n");
    } */

    /* Saída */
    /* printf("\nMatriz de saída:\n");
    for(int i = 0; i < dimensao; i++){
        for(int j = 0; j < dimensao; j++){
            printf("%.1f ", saida[i*dimensao + j]);
        }
        printf("\n");
    } 
    printf("\n"); */

    /* Início da contagem de tempo de finalização do programa */
    GET_TIME(tempoInicio);

    /* Liberação da memória */
    free(matriz1);
    free(matriz2);
    free(saida);
    free(tid);
    free(args);

    /* Fim da contagem de tempo de finalização do programa */
    GET_TIME(tempoFim);
    deltaTempo = tempoFim - tempoInicio;
    printf("Tempo de finalização do programa: %lf\n\n", deltaTempo);

    return 0;
}

/* Função que as threads executarão */
void *multiplica(void *arg){

    tArgs *args = (tArgs *) arg;

    /* Comentado para facilitar a visualização dos resultados esperados pelo exercício */
    /* printf("Thread: %d\n", args->id); */
    
    /* Intercalação das linhas da matriz */
    for(int id = args->id; id < args->dimensao; id += nThreads){ /* Identificadores */
        for(int linha = 0; linha < args->dimensao; linha++){ /* Linhas */
            for(int coluna = 0; coluna < args->dimensao; coluna++){ /* Colunas */
                saida[linha*(args->dimensao) + coluna] += matriz1[linha*(args->dimensao) + coluna] * matriz2[linha*(args->dimensao) + coluna];
            }
        }
    }

    pthread_exit(NULL);
}

/* Dimensão/ Threads -> Menor tempo

Dimensão: 500
Threads: 1 -> Tempo: 0.786247
Threads: 2 -> Tempo: 0.435553
===> Aceleração: 1.80516952013

Dimensão: 1000
Threads: 1 -> Tempo: 6.529348
Threads: 2 -> Tempo: 3.547579
===> Aceleração: 1.84050813245


Dimensão: 2000
Threads: 1 -> Tempo: 51.781243
Threads: 2 -> Tempo: 29.647474
===> Aceleração: 1.74656508679


--------------------------

Dimensão: 500
Threads: 1 -> Tempo: 0.786247
Threads: 4 -> Tempo: 0.407184
===> Aceleração: 1.9309378561

Dimensão: 1000
Threads: 1 -> Tempo: 6.529348
Threads: 4 -> Tempo: 3.242182
===> Aceleração: 2.01387460667


Dimensão: 2000
Threads: 1 -> Tempo: 51.781243
Threads: 4 -> Tempo: 27.473154
===> Aceleração: 1.88479426134

*/