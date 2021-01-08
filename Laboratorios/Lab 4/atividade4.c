/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 2 - Laboratório: 4, Atividade 4 */
/* Codigo: Uso de variáveis de condição e suas operações básicas para sincronização por condição */
/* Feito por Rafael da Silva Fernandes */

/* Condição lógica da aplicação:
 * Como as threads 3 e 4 só podem começar após o términos das 1 e 2,
 * foi criada a variável universal "contador", que será incrementada em 1 nas threads 1 e 2,
 * dessa forma, as threads só poderão ser executadas se contador foi igual à 2,
 * caso não seja, elas serão bloqueadas. */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NTHREADS 4

/* Variáveis globais */
int contador = 0; // Variável de controle para que T1 e T2 sejam executadas antes de T3 e T4
pthread_mutex_t mutex; // Variável de lock para exclusão mútua
pthread_cond_t cond; // Variável de condição

/* Funções utilizadas */
void *T1(void *t); // Imprime "Tudo bem?"
void *T2(void *t); // Imprime "Bom dia!"
void *T3(void *t); // Imprime "Até mais!"
void *T4(void *t); // Imprime "Boa tarde!"


/* Fluxo principal */
int main(void){

    pthread_t threads[NTHREADS]; // Array das threads de tamanho NTHREADS

    /* Inicializa o lock de exclusão mútua e a variável de condiçaõ */
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    /* Criação das threads */
    pthread_create(&threads[0], NULL, T1, NULL);
    pthread_create(&threads[1], NULL, T2, NULL);
    pthread_create(&threads[2], NULL, T3, NULL);
    pthread_create(&threads[3], NULL, T4, NULL);

    /* Espera todas as threads terminarem */
    for(int i = 0; i < NTHREADS; i++){
        pthread_join(threads[i], NULL);
    }

    printf("\nFIM\n");

    /* Desloca variáveis e termina o programa */
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}


/* Thread 1 - imprime "Tudo bem?" */
void *T1(void *t){
    
    printf("T1: Comecei\n");

    /* Espera 1 segundo */
    sleep(1);

    printf("============== Tudo bem?\n");

    /* Início da seção crítica */
    pthread_mutex_lock(&mutex);
    contador++;
    
    if(contador == 2){
        printf("T1: contador = %d, vai sinalizar a condição.\n", contador);
        pthread_cond_broadcast(&cond);
    }

    /* Fim da seção crítica */
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}


/* Thread 2 - imprime "Bom dia!" */
void *T2(void *t){
    
    printf("T2: Comecei\n");

    /* Espera 1 segundo */
    sleep(1);

    printf("============== Bom dia!\n");

    /* Início da seção crítica */
    pthread_mutex_lock(&mutex);
    contador++;
    
    if(contador == 2){
        printf("T2: contador = %d, vai sinalizar a condição.\n", contador);
        pthread_cond_broadcast(&cond);
    }

    /* Fim da seção crítica */
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}


/* Thread 3 - imprime "Boa tarde!" */
void *T3(void *t){
    
    printf("T3: Comecei\n");

    /* Início da seção crítica */
    pthread_mutex_lock(&mutex);

    if(contador < 2){
        printf("T3: contador = %d, vai se bloquear...\n", contador);
        pthread_cond_wait(&cond, &mutex);
        printf("T3: sinal recebido e mutex realocado, contador = %d\n", contador);
    }

    printf("============== Boa tarde!\n");

    /* Fim da seção crítica */
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}


/* Thread 4 - imprime "Até mais!" */
void *T4(void *t){
    
    printf("T4: Comecei\n");

    /* Início da seção crítica */
    pthread_mutex_lock(&mutex);

    if(contador < 2){
        printf("T4: contador = %d, vai se bloquear...\n", contador);
        pthread_cond_wait(&cond, &mutex);
        printf("T4: sinal recebido e mutex realocado, contador = %d\n", contador);
    }

    printf("============== Até mais!\n");

    /* Fim da seção crítica */
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}