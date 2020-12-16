/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 3, Atividade 1 */
/* Código: Cálculo sequencial de uma série de valores reais que aproxima o valor de π */
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


/* Variáveis globais */
long long int dimensao; /* Dimensão do vetor de entrada */
double *vetor; /* Vetor de entrada */


/* Fluxo principal */
int main(int argc, char *argv[]){

    double pi = 0; /* Valor de π (pi) que iremos calcular */
    double denominador; /* Denominador para a extrapolação de Richardson na fórmula de Leibniz */
    
    /* Recebe e valida os parâmetros de entrada */
    if(argc < 2){
        fprintf(stderr, "Digite: %s <dimensão do vetor>\n", argv[0]);
        return 1;
    }

    dimensao = atoll(argv[1]);

    /* Aloca o vetor de entrada */
    vetor = (double *) malloc(sizeof(double) * dimensao);

    if(vetor == NULL){
        fprintf(stderr, "ERRO -- malloc vetor\n");
        return 2;
    }

    /* Preenche o vetor de entrada e realiza a extrapolação de Richardson na fórmula de Leibniz para pi */
    for(long long int i = 0; i < dimensao; i++){
        denominador = ((4*i + 1) * (4*i + 3));
        vetor[i] = 2/ denominador;
        pi += vetor[i];
    }

    pi *= 4;
    
    /* Exibição dos resultados */
    printf("%.15lf => valor de pi calculado por mim\n", pi);
    printf("%.15lf => valor de pi pela math.h\n", M_PI);

    /* Libera as áreas de memória alocadas */
    free(vetor);

    return 0;
}