// Disciplina: Computação Concorrente  
// Prof.: Silvana Rossetto  
// Módulo 3 - Trabalho 3
// Feito por Rafael da Silva Fernandes

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

// Necessidade de 3 threads para execução do programa
// 1 para cada padrão pedido 
#define NTHREADS 3

// Valor arbitrário pois não consegui resolver segmentation fault 
#define MAX 8000


// Variáveis globais
int conta_identicos; // maior quantidade de valores idênticos seguidos
int posicao_identicos; // posição inicial da maior quantidade de valores idênticos seguidos
int valor_identico; // valor com maior quantidade de aparições seguidas
int conta_tripla; // quantidade de sequências contínuas de tamanho 3
int conta_012345; // quantidade de sequências <012345>
int buffer[MAX]; // buffer de entrada
long long int M; // dimensão do buffer de entrada
pthread_mutex_t mutex; // lock para exclusão mútua


// Funções utilizadas
void* busca_identicos(void *arg); // imprime maior sequência de valores idênticos
void* busca_tripla(void *arg); // conta quantidade de sequências contínuas de tamanho 3
void* busca_012345(void *arg); // conta quantidade de sequências <012345>


// Fluxo principal
int main(int argc, char *argv[]){

    // Variáveis para contagem de tempo
    double tempoInicio, tempoFim, deltaTempo;
    GET_TIME(tempoInicio); // início da contagem de tempo

    // Importanto os arquivos .txt
    FILE *vetor1 = fopen("vetor1.txt", "r");
    FILE *vetor2 = fopen("vetor2.txt", "r");
    FILE *vetor3 = fopen("vetor3.txt", "r");
    FILE *vetor4 = fopen("vetor4.txt", "r");
    FILE *vetor5 = fopen("vetor5.txt", "r");    
    FILE *vetor6 = fopen("vetor6.txt", "r");     

    // >>> Necessário alterar primeiro parâmetro para testar outro arquivo <<<
    fscanf(vetor1, "%d", &buffer[0]);
    M = buffer[0];

    for(int i = 1; i <= M; i++){
        // >>> Necessário alterar primeiro parâmetro para testar outro arquivo <<<
        fscanf(vetor1, "%d", &buffer[i]); 
    }

    // Identificadores das threads no sistema
    pthread_t threads[NTHREADS]; // Array das threads de tamanho NTHREADS

    // Inicia o mutex (lock de exclusão mútua)
    pthread_mutex_init(&mutex, NULL);

    // Criação das threads
    pthread_create(&threads[0], NULL, busca_identicos, NULL);
    pthread_create(&threads[1], NULL, busca_tripla, NULL);
    pthread_create(&threads[2], NULL, busca_012345, NULL);

    // Espera todas as threads terminarem
    for(int i = 0; i < NTHREADS; i++){
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    GET_TIME(tempoFim); // fim da contagem de tempo
    deltaTempo = tempoFim - tempoInicio;
    printf("\nTempo decorrido = %lf\n\n", deltaTempo);

    if(posicao_identicos == -1){
        printf("Não há sequência de números idênticos.\n");
    } else{
        printf("Maior sequência de valores idênticos: %d %d %d\n", posicao_identicos, conta_identicos, valor_identico);
    }

    printf("Maior quantidade de triplas: %d\n", conta_tripla);
    printf("Quantidade de ocorrências da sequência <012345>: %d\n", conta_012345);

    fclose(vetor1);
    fclose(vetor2);
    fclose(vetor3);
    fclose(vetor4);
    fclose(vetor5);
    fclose(vetor6);

    return 0;
}


// Busca maior sequência de idênticos
void *busca_identicos(void *arg){

    int conta_0 = 0; // quantidade de valores 0's seguidos
    int conta_1 = 0; // quantidade de valores 1's seguidos
    int conta_2 = 0; // quantidade de valores 2's seguidos
    int conta_3 = 0; // quantidade de valores 3's seguidos
    int conta_4 = 0; // quantidade de valores 4's seguidos
    int conta_5 = 0; // quantidade de valores 5's seguidos

    // Início seção crítica
    pthread_mutex_lock(&mutex);

    conta_identicos = 0;
    posicao_identicos = -1;

    for(int i = 1; i <= M; i++){

        // Procurando sequência de 0's
        if((buffer[i] == 0) && (buffer[i+1] == 0)){
            if(posicao_identicos == -1){
                posicao_identicos = i;
            }
            conta_0++;
        } 

        // Procurando sequência de 1's
        if((buffer[i] == 1) && (buffer[i+1] == 1)){
            if(posicao_identicos == -1){
                posicao_identicos = i;
            }
            conta_1++; 
        } 
        
        // Procurando sequência de 2's
        if((buffer[i] == 2) && (buffer[i+1] == 2)){
            if(posicao_identicos == -1){
                posicao_identicos = i;
            }
            conta_2++; 
        } 
        
        // Procurando sequência de 3's
        if((buffer[i] == 3) && (buffer[i+1] == 3)){
            if(posicao_identicos == -1){

                posicao_identicos= i;
            }
            conta_3++; 
        } 
        
        // Procurando sequência de 4's
        if((buffer[i] == 4) && (buffer[i+1] == 4)){
            if(posicao_identicos == -1){
                posicao_identicos = i;
            }
            conta_4++; 
        } 
        
        // Procurando sequência de 5's
        if((buffer[i] == 5) && (buffer[i+1] == 5)){
            if(posicao_identicos == -1){
                posicao_identicos = i;
            }
            conta_5++; 
        }
    }

    int total[6] = {conta_0, conta_1, conta_2, conta_3, conta_4, conta_5};

    // Pegando tamanho e valor da maior sequêcia de números idênticos
    for(int i = 0; i < 6; i++){
        if(total[i] > conta_identicos){
            conta_identicos = total[i] + 1;
            valor_identico = i;
        }
    }

    // Fim seção crítica
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}


// Conta quantidade de sequências de tamanho 3
void *busca_tripla(void *arg){

    // Início seção crítica
    pthread_mutex_unlock(&mutex);

    for(int i = 1; i <= M; i++){
        if((buffer[i] == buffer[i+1]) && (buffer[i+1] == buffer[i+2])){
            conta_tripla++;
            i += 2; // excluindo casos como [0, 0, 0 ,0] contarem como 2 sequências, por exemplo
        } 
    }

    // Fim seção crítica
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}


// Quantidade de sequências <012345>
void *busca_012345(void *arg){

    // Início seção crítica
    pthread_mutex_lock(&mutex);

    conta_012345 = 0;

    for(int i = 1; i <= M; i++){
        if((buffer[i] == 0) && (buffer[i+1] == 1) && (buffer[i+2] == 2) && (buffer[i+3] == 3) && (buffer[i+4] == 4) && (buffer[i+5] == 5)){
            conta_012345++;
        }
    }

    // Fim seção crítica
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}