/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 6 - Atividade 5 */
/* Codigo: programa concorrente em Java para somar dois vetores, fazendo C = A + B. */
/* Feito por Rafael da Silva Fernandes */


// // Classe da estrutura de dados compartilhada entre as threads
class Soma{

    // Recursos compartilhados
    static final int N = 20; // tamanho dos vetores
    
    public int[] A = new int[N];
    public int[] B = new int[N];
    public int[] C = new int[N];

    // Construtor
    public Soma(){

        // Preenche vetor A com 1's
        for(int i = 0; i < N; i++){
            this.A[i] = 1;
        }

        // Preenche vetor B com 2's
        for(int i = 0; i < N; i++){
            this.B[i] = 2;
        }
    }

    // Soma dos vetores A e B
    public void soma(){
        for(int i = 0; i < N; i++){
            this.C[i] = this.A[i] + this.B[i];
        }
    }
}

// Classe que estende Thread e implementa a tarefa de cada thread
class T extends Thread{

    private int id; // identificador da thread
    Soma soma; // objeto compartilhado com outras threads

    // Construtor
    public T(int tid, Soma soma){
        this.id = tid;
        this.soma = soma;
    }

    // Método main da thread
    public void run(){
        System.out.println("Thread " + this.id + " iniciou");
        this.soma.soma();
        System.out.println("Thread " + this.id + " terminou");
    }
}

// Classe do fluxo central
class Atividade5{

    static final int NTHREADS = 4; // quantidade de threads

    // Fluxo central
    public static void main(String[] arg){

        // Reserva espaço para um vetor de threads
        Thread[] threads = new Thread[NTHREADS];

        // Criação instância do recurso compartilhado entre as threads
        Soma soma = new Soma();

        // Imprime vetor A
        System.out.print("Vetor A = ");
        for(int i = 0; i < soma.N; i++){
            if(i == (soma.N - 1)){
                System.out.println(soma.A[i]);
            } else{
                System.out.print(soma.A[i] + " ");
            }
        }

        // Imprime vetor B
        System.out.print("Vetor B = ");
        for(int i = 0; i < soma.N; i++){
            if(i == (soma.N - 1)){
                System.out.println(soma.B[i]);
            } else{
                System.out.print(soma.B[i] + " ");
            }
        }

        // Cria as threads
        for(int i = 0; i < threads.length; i++){
            final String message = "Criação thread #" + i;
            threads[i] = new T(i, soma);
        }

        // Inicia as threads
        for(int i = 0; i < threads.length; i++){
            threads[i].start();
        }

        // Espera pelo término de todas as threads
        for(int i = 0; i < threads.length; i++){
            try{ threads[i].join(); } catch(InterruptedException e){ return; }
        }

        // Imprime vetor C = A + B
        System.out.print("Vetor C = ");
        for(int i = 0; i < soma.N; i++){
            if(i == (soma.N - 1)){
                System.out.println(soma.C[i]);
            } else{
                System.out.print(soma.C[i] + " ");
            }
        }
    }
}