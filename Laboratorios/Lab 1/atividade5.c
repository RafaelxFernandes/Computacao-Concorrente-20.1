/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 1, Atividade 5 */
/* Código: Incrementar de 1 cada elemento de um vetor de N elementos */
/* Feito por Rafael da Silva Fernandes */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* Número total de elementos do vetor */
#define N 32

/* Vetor cujos elementos serão incrementados */
int vetor[N];


/* Função que incrementa em 1 os valores do vetor */
void *incrementa(void *arg){
    int id = *(int *) arg;

    if(id == 1){ /* Incrementa os valores nas posições pares */
        for(int i = 0; i < N; i += 2){
            printf("Incrementei o valor na posição: %d\n", i);
            vetor[i]++;
        }
    } if(id == 2){ /* Incrementa os valores nas posições ímpares */
        for(int i = 1; i < N; i += 2){
            printf("Incrementei o valor na posição: %d\n", i);
            vetor[i]++;
        }
    }

    pthread_exit(NULL);
}

/* Função principal */
int main(){

    /* Imprime valores iniciais do vetor */
    printf("Valores iniciais do vetor: ");
    for(int i = 0; i < N; i++){
        vetor[i] = i + 1;
        printf("%d ", vetor[i]);
    }
    printf("\n\n");

    pthread_t tid[2]; /* Identificador da thread no sistema */
    int identificador[2] = {1, 2}; /* Identificador local da thread */

    /* Cria a thread que incrementa os valores em posições par do vetor */        
    if(pthread_create(&tid[0], NULL, incrementa, (void *) &identificador[0])){
        printf("ERRO -- pthread_create incrementa Par\n");
    } 

    /* Cria a thread que incrementa os valores em posições ímpar do vetor */        
    if(pthread_create(&tid[1], NULL, incrementa, (void *) &identificador[1])){
        printf("ERRO -- pthread_create incrementa Impar\n");
    } 

    /* Espera as threads terminarem */
    for(int i = 0; i < 2; i++){
        if(pthread_join(tid[i], NULL)){
            printf("ERRO --  pthread_join\n");
        }
    }

    /* Imprime valores finais do vetor */
    printf("\nValores finais do vetor: ");
    for(int i = 0; i < N; i++){
        printf("%d ", vetor[i]);
    }
    printf("\n");

    /* Desvincula o término da main do término do programa */
    pthread_exit(NULL);

    return 0;
}