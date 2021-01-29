/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 7 - Atividade 2 */
/* Codigo: inserção e remoção de elementos em um buffer (versão concorrente) */
/* Feito por Rafael da Silva Fernandes */

// Monitor
class PC{
    private int count, in, out; // variáveis de estado
    public int N = 15; // tamanho do buffer
    public int[] buffer = new int[N];

    // Construtor
    PC(){
        this.count = 0;
        this.in = 0;
        this.out = 0;
    }

    // Insere um elemento no buffer ou bloqueia a thread caso o buffer esteja cheio
    public synchronized void Insere(int item, int id){
        try{
            while(count == N){
                System.out.println("pc.insereBloqueado(" + id + ")");
                wait();
            }

            buffer[in] = item;
            in = (in + 1) % N;
            count++;

            System.out.println("# P[" + id + "] inseriu " + id);

            // Imprime buffer
            System.out.print("# Buffer atualizado = ");
            for(int i = 0; i < N; i++){
                System.out.print(buffer[i] + " ");
            }
            System.out.println("");

            notifyAll();
        } catch(InterruptedException e){ }
    }

    // Retira um elemento no buffer ou bloqueia a thread caso o Buffer esteja vazio
    public synchronized void Retira(int id){
        int item;
        
        try{
            while(count == 0){
                System.out.println("pc.retiraBloqueado(" + id + ")");
                wait();
            }

            item = buffer[out];
            buffer[out] = 0;
            out = (out + 1) % N;
            count--;

            System.out.println("# C[" + id + "] consumiu " + item);

            // Imprime buffer
            System.out.print("# Buffer atualizado = ");
            for(int i = 0; i < N; i++){
                System.out.print(buffer[i] + " ");
            }
            System.out.println("");

            notifyAll();
        } catch(InterruptedException e){ }
    }
}

// Produtor
class Produtor extends Thread{
    int id; // identificador da thread
    int delay; // atraso
    PC monitor; // objeto monitor

    // Construtor
    Produtor(int id, int delay, PC monitor){
        this.id = id;
        this.delay = delay;
        this.monitor = monitor;
    }

    // Método executado pela thread
    public void run(){
        try{
            while(true){
                this.monitor.Insere(id, id);
                sleep(this.delay);
            }
        } catch(InterruptedException e){ }
    }
}

// Consumidor
class Consumidor extends Thread{
    int id; // identificador da thread
    int delay; // atraso
    PC monitor; // objeto monitor

    // Construtor
    Consumidor(int id, int delay, PC monitor){
        this.id = id;
        this.delay = delay;
        this.monitor = monitor;
    }

    // Método executado pela thread
    public void run(){
        try{
            while(true){
                this.monitor.Retira(id);
                sleep(this.delay);
            }
        } catch(InterruptedException e){ }
    }
}

// Classe principal
public class Atividade2 {
    static final int P = 5; // número de threads produtoras
    static final int C = 5; // número de threads consumidoras

    public static void main(String[] args){
        PC monitor = new PC(); // monitor
        Produtor[] p = new Produtor[P]; // threads produtoras
        Consumidor[] c = new Consumidor[C]; // threads consumidoras

        // Inicia o log de saída
        System.out.println("import verificaPC");
        System.out.println("pc = verificaPC.PC()");

        // Cria e inicia threads produtoras
        for(int i = 0; i < P; i++){
            p[i] = new Produtor(i+1, (i+1)*500, monitor);
            p[i].start();
        }

        // Cria e inicia threads consumidoras
        for(int i = 0; i < C; i++){
            c[i] = new Consumidor(i+1, (i+1)*500, monitor);
            c[i].start();
        }
    }   
}