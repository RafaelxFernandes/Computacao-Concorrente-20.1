/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 3, Atividade 2 */
/* Código: Cálculo concorrente de uma série de valores reais que aproxima o valor de π */
/* Feito por Rafael da Silva Fernandes */

/* A estratégia usada para minimizar os erros numéricos foi de aplicar a extrapolação de Richardson... */
/* ... em cima da fórmula de Leibniz para π */
/* Abaixo seguem os links utilizados como referência:*/
/* https://abelsiqueira.github.io/disciplinas/cm141/2016s2/renata.pdf */
/* https://pt.wikipedia.org/wiki/F%C3%B3rmula_de_Leibniz_para_%CF%80 */
/* https://homepages.dcc.ufmg.br/~assuncao/an/Integracao-2.pdf */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "timer.h"


/* Variáveis globais */
long long int dimensao; /* Dimensão do vetor de entrada */
double *vetor; /* Vetor de entrada */
int nThreads; /* Número de threads */


/* Funções utilizadas */
void *soma(void *arg);

/* Fluxo principal */
int main(int argc, char *argv[]){

    double piSequencial = 0; /* Valor de π (pi) que iremos calcular */
    double denominadorSequencial; /* Denominador para a extrapolação de Richardson na fórmula de Leibniz */
    
    double piConcorrente = 0;
    // double denominadorConcorrente;
    
    double tempoInicio, tempoFim, deltaTempo; /* Variáveis para contagem de tempo */
    
    pthread_t *tid; /* Identificadores das threads no sistema */
    double *retorno; /* Valor do retorno das threads */

    /* Recebe e valida os parâmetros de entrada */
    if(argc < 3){
        fprintf(stderr, "Digite: %s <dimensão do vetor> <número de threads>\n", argv[0]);
        return 1;
    }

    dimensao = atoll(argv[1]);
    nThreads = atoi(argv[2]);

    /* Aloca o vetor de entrada */
    vetor = (double *) malloc(sizeof(double) * dimensao);

    if(vetor == NULL){
        fprintf(stderr, "ERRO -- malloc vetor\n");
        return 2;
    }

    /* Soma sequencial */
    GET_TIME(tempoInicio);

    /* Preenche o vetor de entrada e realiza a extrapolação de Richardson na fórmula de Leibniz para pi */
    for(long long int i = 0; i < dimensao; i++){
        denominadorSequencial = ((4*i + 1) * (4*i + 3));
        vetor[i] = 2/ denominadorSequencial;
        piSequencial += vetor[i];
    }

    piSequencial *= 4;

    GET_TIME(tempoFim);
    deltaTempo = tempoFim - tempoInicio;
    printf("Tempo sequencial: %lf\n", deltaTempo);

    /* Soma concorrente */
    GET_TIME(tempoInicio);

    tid = (pthread_t *) malloc(sizeof(pthread_t) * nThreads);

    if(tid == NULL){
        fprintf(stderr,"ERRO -- malloc tid\n");
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
        piConcorrente += *retorno;
        free(retorno);
    }


    GET_TIME(tempoFim);
    deltaTempo = tempoFim - tempoInicio;
    printf("Tempo concorrente: %lf\n", deltaTempo);
    
    /* Exibição dos resultados */
    printf("%.15lf => valor de pi sequencial\n", piSequencial);
    printf("%.15lf => valor concorrente de pi\n", piConcorrente);
    printf("%.15lf => valor de pi pela math.h\n", M_PI);

    /* Libera as áreas de memória alocadas */
    free(vetor);
    free(tid);

    return 0;
}

/* Função que as threads executarão */
void *soma(void *arg){

    long int id = (long int) arg; /* Identificador da thread */
    long int tamanhoBloco = dimensao/ nThreads; /* Tamanho do bloco de cada thread */
    long int posicaoInicio = id * tamanhoBloco; /* Elemento inicial do bloco */
    long int posicaoFim; /* Elemento final (não processado) do bloco */
    double denominadorLocal;
    double *piLocal;

    piLocal = (double *) malloc(sizeof(double));

    if(piLocal == NULL){
        fprintf(stderr, "ERRO -- malloc piLocal\n");
        exit(1);
    }

    *piLocal = 0;

    /* Tratamento do resto (se houver) */
    if(id == (nThreads - 1)){
        posicaoFim = dimensao;
    } else {
        posicaoFim = posicaoInicio + tamanhoBloco;
    }

    for(long int i = posicaoInicio; i < posicaoFim; i++){
        denominadorLocal = ((4*i + 1) * (4*i + 3));
        vetor[i] = 2/ denominadorLocal;
        *piLocal += vetor[i];
    }

    *piLocal *= 4;

    pthread_exit((void *) piLocal);
}