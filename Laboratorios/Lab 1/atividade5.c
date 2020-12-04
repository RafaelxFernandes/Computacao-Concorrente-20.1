/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 1, Atividade 5 */
/* Código: Incrementar de 1 cada elemento de um vetor de NTHREADS elementos */
/* Feito por Rafael da Silva Fernandes */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* Número total de threads a serem criadas */
#define NTHREADS 16


/* Estrutura de dados que armazena os argumentos da thread */
typedef struct{
    int *array; /* Vetor */
    int id; /* Posição do elemento no vetor */
} t_Args;


/* Função que incrementa em 1 os valores do vetor */
void *incrementa(void *arg){
    t_Args *args = (t_Args *) arg;

    args->array[args->id]++;
    printf("Incrementei o valor na posição: %d de %d\n", args->id, NTHREADS);
    free(arg); /* Liberação da alocação feita na main */

    pthread_exit(NULL);   
}


/* Função principal */
int main(){
    int vetor[NTHREADS]; /* Vetor cujos elementos serão incrementados */

    /* Imprime valores iniciais do vetor */
    printf("Valores iniciais do vetor: ");
    for(int i = 0; i < NTHREADS; i++){
        vetor[i] = i + 1;
        printf("%d ", vetor[i]);
    }
    printf("\n\n");

    pthread_t tid[NTHREADS]; /* Identificador da thread no sistema */
    t_Args *arg; /* Recebe os argumentos para a thread */

    /* Cria a thread que incrementa os valores em posições par do vetor */
    for(int i = 0; i < NTHREADS; i += 2){

        printf("--- Aloca e preenche argumentos para a thread %d\n", i);
        arg = malloc(sizeof(t_Args));
        if(arg == NULL){
            printf("ERRO -- malloc()\n");
            exit(-1);
        }

        arg->id = i;
        arg->array = vetor;
        
        if(pthread_create(&tid[i], NULL, incrementa, (void *) arg)){
            printf("ERRO -- pthread_create incrementa Par\n");
        } 
    }

    /* Cria a thread que incrementa os valores em posições ímpar do vetor */
    for(int i = 1; i < NTHREADS; i += 2){

        printf("--- Aloca e preenche argumentos para thread: %d\n", i);
        arg = malloc(sizeof(t_Args));
        if(arg == NULL){
            printf("ERRO -- malloc()\n");
            exit(-1);
        }

        arg->id = i;
        arg->array = vetor;
        
        if(pthread_create(&tid[i], NULL, incrementa, (void *) arg)){
            printf("ERRO -- pthread_create incrementa Impar\n");
        } 
    }

    /* Espera as threads terminarem */
    for(int i = 0; i < NTHREADS; i++){
        if(pthread_join(tid[i], NULL)){
            printf("ERRO --  pthread_join\n");
        }
    }

    /* Imprime valores finais do vetor */
    printf("\nValores finais do vetor: ");
    for(int i = 0; i < NTHREADS; i++){
        printf("%d ", vetor[i]);
    }
    printf("\n");

    /* Desvincula o término da main do término do programa */
    pthread_exit(NULL);

    return 0;
}