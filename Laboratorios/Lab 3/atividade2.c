/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 3, Atividade 2 */
/* Código: Cálculo concorrente de uma série de valores reais que aproxima o valor de π */
/* Feito por Rafael da Silva Fernandes */

/* A estratégia usada para minimizar os erros numéricos foi de aplicar a extrapolação de Richardson... 
   ... em cima da fórmula de Leibniz para π. Em análise numérica, a extrapolação de Richardson é...
   ... um método de aceleração para sequências, usado para melhorar a convergência de somas estimadas...
   ... do valor A* = lim h->0 A(h), em que A(h) = A* + C*(h^n) + O(h^(n+1)), ...
   ... e a extrapolação de Richardson para A(h) é R(h) = A* + O(h^(n+1)). Em geral, é mais fácil...
   ... obter uma precisão dada utilizando R(h) do que A(h'), sendo h' < h.
    Em essência, dado um valor de A(h) para vários valores de h, podemos estimar A*...
    ... extrapolando as estimativas para h = 0. No Método do Trapézio, por exemplo, fornece aproximações...
    ... de ordem 2; nos métodos de Simpsons fornece aproximações de ordem 4. */

/* Abaixo seguem os links utilizados como referência:*/
/* https://abelsiqueira.github.io/disciplinas/cm141/2016s2/renata.pdf */
/* https://pt.wikipedia.org/wiki/F%C3%B3rmula_de_Leibniz_para_%CF%80 */
/* https://homepages.dcc.ufmg.br/~assuncao/an/Integracao-2.pdf */
/* https://en.wikipedia.org/wiki/Richardson_extrapolation */
/* http://www.mat.ufrgs.br/~guidi/grad/MAT01032/calculo_numerico.cap7.pdf */

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


/*
N = 10^3
1 thread: 0.000033
2 thread: 0.000671 => Aceleração: 0.04918032786
4 thread: 0.000887 => Aceleração: 0.03720405862

-----
N = 10^5
1 thread: 0.003500
2 thread: 0.001033 => Aceleração: 3.38818973863
4 thread: 0.001263 => Aceleração: 2.7711797308

----

N = 10^7
1 thread: 0.155453
2 thread: 0.066016 => Aceleração: 2.35477762967
4 thread: 0.037330 => Aceleração: 4.16429145459

----

N = 10^8
1 thread: 1.503315 
2 thread: 0.668863 => Aceleração: 2.24756788759
4 thread: 0.346237 => Aceleração: 4.34186698706


*/
