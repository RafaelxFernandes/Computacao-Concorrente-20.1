/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 2 - Laboratório: 5, Atividade 2 */
/* Código: aplicação com o padrão leitores/ escritores, em que
    quando um escritor desejar escrever, nenhum leitor
    pode iniciar uma leitura enquanto o escritor nao for atendido.
    Leitora: percorre o vetor de inteiros e imprime na tela seu conteudo e a média dos valores encontrados;
    Escritora: modifica o conteudo do vetor escrevendo o valor do seu identificador
    de thread na aplicação na primeira e última posição do vetor, 
    e o dobro desse valor nas demais posições. */
/* Feito por Rafael da Silva Fernandes */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N 4 // número de threads leitoras
#define M 8 // número de threads escritoras
#define TAM 20 // tamanho do vetor

// Variáveis globais
int leitoras = 0; // contador de threads lendo
int escritoras = 0; // contador de threads escrevendo
int escritorasEsperando = 0; // estado adicional que contabiliza escritoras esperando para escrever
int vetor[TAM]; // vetor a ser alterado pelas threads
pthread_mutex_t mutex; // variável de lock para exclusão mútua
pthread_cond_t condicaoLeitoras, condicaoEscritoras; // variáveis para sincronização

// Funções utilizadas
void iniciaLeitura(int id); // entrada leitura
void fimLeitura(int id); // saída leitura
void iniciaEscrita(int id); // entrada escrita
void fimEscrita(int id); // saída escrita
void *leitora(void *arg); // função executada pelas threads leitoras
void *escritora(void *arg); // funções executada pelas threads escritoras


// Fluxo principal
int main(void){

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

    // Identificadores das threads
    pthread_t tid[N + M];
    int id[N + M];

    // Inicializa as variáveis de sincronização
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condicaoLeitoras, NULL);
    pthread_cond_init(&condicaoEscritoras, NULL);

    // Cria as threads leitoras
    for(int i = 0; i < N; i++){
        id[i] = i + 1;

        if(pthread_create(&tid[i], NULL, leitora, (void *) &id[i])){
            exit(-1);
        }
    }

    // Cria as threads escritoras
    for(int i = 0; i < M; i++){
        id[i + N] = i + 1;

        if(pthread_create(&tid[i + N], NULL, escritora, (void *) &id[i + N])){
            exit(-1);
        }
    }

    // Espera todas as threads completarem
    // for(int i = 0; i < (N + M); i++){
    //     pthread_join(tid[i], NULL);
    // }

    // printf("FIM\n");

    // Desaloca variáveis e termina
    // pthread_mutex_destroy(&mutex);
    // pthread_cond_destroy(&condicaoLeitoras);
    // pthread_cond_destroy(&condicaoEscritoras);
    
    // return 0;

    pthread_exit(NULL);
}

// Entrada leitura
void iniciaLeitura(int id){

    // Início seção crítica
    pthread_mutex_lock(&mutex);
    printf("L[%d] quer ler\n", id);

    while((escritoras > 0) ||(escritorasEsperando > 0)){
        printf("L[%d] bloqueou\n", id);
        pthread_cond_wait(&condicaoLeitoras, &mutex);
        printf("L[%d] desbloqueou\n", id);
    }

    leitoras++;

    // Fim seção crítica
    pthread_mutex_unlock(&mutex);
}

// Saída leitura
void fimLeitura(int id){

    // Início seção crítica
    pthread_mutex_lock(&mutex);
    printf("L[%d] terminou de ler\n", id);
    leitoras--;

    pthread_cond_signal(&condicaoEscritoras); // Libera 1 thread escritora

    // Fim seção crítica
    pthread_mutex_unlock(&mutex);
}

// Entrada escrita
void iniciaEscrita(int id){

    // Início seção crítica
    pthread_mutex_lock(&mutex);
    printf("E[%d] quer escrever\n", id);
    escritorasEsperando++;
    printf("Escritoras esperando = %d\n", escritorasEsperando);

    while((leitoras > 0) || (escritoras > 0)){
        printf("E[%d] bloqueou\n", id);
        pthread_cond_wait(&condicaoEscritoras, &mutex);
        printf("E[%d] desbloqueou\n", id);
    }

    escritorasEsperando--;
    escritoras++;

    // Fim seção crítica
    pthread_mutex_unlock(&mutex);
}

// Saída escrita
void fimEscrita(int id){

    // Início seção crítica
    pthread_mutex_lock(&mutex);
    printf("E[%d] terminou de escrever\n", id);
    escritoras--;
    printf("Escritoras esperando = %d\n", escritorasEsperando);

    pthread_cond_broadcast(&condicaoEscritoras); // Libera todas as threads escritoras

    if(escritoras == 0){
        pthread_cond_broadcast(&condicaoLeitoras); // Libera todas as threads leitoras
    }
    
    // Fim seção crítica
    pthread_mutex_unlock(&mutex);
}

// Função executada pelas threads leitoras:
// percorre o vetor de inteiros e imprime na tela seu conteudo e a média dos valores encontrados
void *leitora(void *arg){
    
    int *id = (int *) arg; 
    float soma = 0; // soma dos valores do vetor
    float media = 0; // média dos valores do vetor

    while(1){
        iniciaLeitura(*id);
        printf("Leitora %d está lendo\n", *id);

        // Início seção crítica
        pthread_mutex_lock(&mutex);

        // Imprime vetor
        printf("\nVetor atual:\n");
        for(int i = 0; i < TAM; i++){
            if(i == (TAM - 1)){
                printf("%d\n", vetor[i]);
            } else{
                printf("%d ", vetor[i]);
            }

            soma += vetor[i];
        }

        media = soma/ TAM;
        printf("Média atual: %f\n\n", media);

        // Necessário zerar os valores de soma e media para obtenção do resultado esperado
        soma = 0;
        media = 0;

        // Fim seção crítica
        pthread_mutex_unlock(&mutex);

        fimLeitura(*id);
        sleep(1);
    }

    free(arg);
    pthread_exit(NULL);
}

// Função executada pelas threads escritoras:
// modifica o conteudo do vetor escrevendo o valor do seu identificador
// de thread na aplicação na primeira e última posição do vetor, 
// e o dobro desse valor nas demais posições.
void *escritora(void *arg){

    int *id = (int *) arg;

    while(1){
        iniciaEscrita(*id);
        printf("Escritora %d está escrevendo\n", *id);

        // Modifica vetor
        // Início seção crítica
        pthread_mutex_lock(&mutex);

        for(int i = 0; i < TAM; i++){
            if((i == 0) || (i == TAM-1)){
                vetor[i] = *id;
            } else{
                vetor[i] = *id * 2;
            }
        }

        // Fim seção crítica
        pthread_mutex_unlock(&mutex);

        fimEscrita(*id);
        sleep(1);
    }

    free(arg);
    pthread_exit(NULL);
}