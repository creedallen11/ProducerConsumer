public class ProduceConsumeSim {
    public static void main(String [] args) {
        
        // SET THESE TO CHANGE SIMULATION
        int M = 15; // Upper bound for request length (seconds)
        int N = 5; // size of buffer & # of consumers
        
        Producer producer = new Producer(N, M);
        producer.start();
        
        for (int i = 1; i <= N; i++)
            new Consumer(producer, i).start();
    }
}
