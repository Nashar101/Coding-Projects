import java.io.*;
import java.net.*;

public class ClientHandler implements Runnable {
    //declare variables
    Socket clientsocket;
    int clientNo;
    Database db1;
    //Constructor
    public ClientHandler (Socket socket, int clientId, Database db) {
        //complete the constructor
        clientsocket = socket;
        clientNo = clientId;
        db1 = db;
    }

    public void run() {
        /*System.out.println("ClientHandler started...");
              Create I/O streams to read/write data, PrintWriter and BufferedReader
              Receive messages from the client and send replies, until the user types "stop"
                  System.out.println("Client sent the artist name " + clientMessage);
                  Request the number of titles from the db
                  Send reply to Client:
                  outToClient.println("Number of titles: " + titlesNum + " records found");

              System.out.println("Client " + clientId + " has disconnected");
              outToClient.println("Connection closed, Goodbye!");
              Close I/O streams and socket*/
        System.out.println("ClientHandler Started...");
        try {
            PrintWriter printWriter = new PrintWriter(clientsocket.getOutputStream(), true);
            BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(clientsocket.getInputStream()));
            String message;
            int titlesNum;
            while(!(message = bufferedReader.readLine()).equals("stop")){
                System.out.println("Client sent the artist name " + message);
                titlesNum = db1.getTitles(message);
                printWriter.println("Number of titles: " + titlesNum + " records found");
            }
            System.out.println("Client " + clientNo + " has disconnected");
            printWriter.println("Connection closed, Goodbye!");
            bufferedReader.close();
            printWriter.close();
            clientsocket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

    }
}
