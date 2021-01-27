/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 7 - Atividade 1 */
/* Codigo: inserção e remoção de elementos em um buffer (versão não concorrente) */
/* Feito por Rafael da Silva Fernandes */

import java.util.Random;

// Classe principal
public class Atividade1 {

    static final int N = 15; // tamanho do buffer
    public static int[] buffer = new int[N]; // buffer de números inteiros

    static Random random = new Random(); // gerador de números aleatórios

    public static void main(String[] args) {

        // Inicializa buffer preenchendo-o com 0's
        for (int i = 0; i < N; i++) {
            buffer[i] = 0;
        }

        // Imprime buffer inicial
        System.out.print("Buffer inicial = ");
        for (int i = 0; i < N; i++) {
            if (i == (N - 1)) {
                System.out.println(buffer[i] + "\n");
            } else {
                System.out.print(buffer[i] + " ");
            }
        }

        // Tenta inserir o valor 1 em N posições do buffer
        for(int i = 0; i < N; i++){
            int aleatorio = random.nextInt(N); // gera número aleatório de 0 à N-1
            System.out.println("Tentando preencher posição " + aleatorio + " do buffer...");
    
            if(buffer[aleatorio] == 0){
                buffer[aleatorio] = 1;
                System.out.println("Posição preenchida com sucesso!");
            } else{
                System.out.println("--- AVISO! Posição " + aleatorio + " já ocupada!");
            }
        }

        // Imprime buffer após inserção de elementos
        System.out.print("\nBuffer após inserções = ");
        for (int i = 0; i < N; i++) {
            if (i == (N - 1)) {
                System.out.println(buffer[i] + "\n");
            } else {
                System.out.print(buffer[i] + " ");
            }
        }

        // Tenta remover elemento em N posições do buffer
        for(int i = 0; i < N; i++){
            int aleatorio = random.nextInt(N); // gera número aleatório de 0 à N-1
            System.out.println("Tentando remover elemento na posição " + aleatorio + " do buffer...");
    
            if(buffer[aleatorio] != 0){
                buffer[aleatorio] = 0;
                System.out.println("Elemento removido com sucesso!");
            } else{
                System.out.println("--- AVISO! Posição " + aleatorio + " já está vazia!");
            }
        }

        // Imprime buffer após inserção de elementos
        System.out.print("\nBuffer apóes remoções = ");
        for (int i = 0; i < N; i++) {
            if (i == (N - 1)) {
                System.out.println(buffer[i] + "\n");
            } else {
                System.out.print(buffer[i] + " ");
            }
        }
    }
}