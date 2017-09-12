class Consumer extends Thread {
 
    Producer producer;
    int ID;
 
    Consumer(Producer p, int ID) {
        producer = p; this.ID = ID;
    }
 
    @Override
    public void run() {
        try {
            while (true) { // Consumer processes requests & simulates w/sleep.
                Request r = producer.processRequest();
                System.out.println("Consumer " + ID + ": " + r);
                sleep(r.duration);
            }
        } catch (InterruptedException e) {
            System.out.println("Consumer " + ID + " failed to run.");
        }
    }
}