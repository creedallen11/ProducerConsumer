import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Random;
// import java.time.ZonedDateTime;
// import java.time.format.DateTimeFormatter;
 
class Producer extends Thread {
 
    static int MAXBUF; // Max size of requestBuffer.
    List<Request> requestBuffer; // List of requests.
    Random rand;
    int nextID = 1; int M; // M is the upper bound of request length (seconds).
    
    public Producer(int N, int M) {
        this.requestBuffer = Collections.synchronizedList(new ArrayList<Request>());
        MAXBUF = N;
        rand = new Random();
        this.M = M;
    }
    
    @Override
    public void run() {
        try {
            while (true) {
                generateRequest();
                sleep(rand.nextInt(2) * 1000); // small time to sleep in between requests
            }
        } catch (InterruptedException e) {
        }
    }
 
    private synchronized void generateRequest() throws InterruptedException {
        while (requestBuffer.size() == MAXBUF)
            wait(); // Wait for a consumer to empty a spot in the buffer.
        Request newRequest = new Request(nextID, (rand.nextInt(M)+1) * 1000);
        System.out.println("Producer: produced request ID " + nextID + ", length "
                + newRequest.duration/1000.0 + " seconds."); //" Current time = " + ZonedDateTime.now().format(DateTimeFormatter.ISO_TIME));
        requestBuffer.add(newRequest);
        nextID++;
        notify(); // Done w/buffer.
    }
 
    // Public because processRequest is called by Consumer
    public synchronized Request processRequest() throws InterruptedException {
        notify();
        while (requestBuffer.isEmpty())
            wait(); // Wait for producer to generate requests.
        Request r = (Request) requestBuffer.remove(0);
        return r;
    }
}
