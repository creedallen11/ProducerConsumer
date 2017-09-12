import java.util.Random;

public class Request {
    int ID;
    int duration;
    
    public Request(int ID, int M) {
        this.ID = ID;
        Random rand = new Random();
        this.duration = rand.nextInt(M);
    }
    
    @Override
    public String toString() {
        return "Assigned request ID " + ID + " processing request for next " + 
                duration / 1000.0 + " seconds.";
    }
}
