package de.wetterstation;

import java.io.IOException;

import de.wetterstation.thread.ScannerThread;
import de.wetterstation.thread.SocketThread;
import de.wetterstation.util.ServerSocket;
import de.wetterstation.util.Socket;

public class WetterServer {

    private ServerSocket serverSocket;
    private boolean stopping = false;

    /**
     * Starts the thread to listen for inputs in the server console
     * Starts listening for incoming connections and handles them
     * The socket is handled in a different thread (SocketThread.java)
     * @param port the port the server should listen for incoming connections on
     */
    public WetterServer(int port) {
        try {
            System.out.println("Wetterstation Server\n");
            this.serverSocket = new ServerSocket(port);
            System.out.println("Starte Server..");

            // start thread for messages from console
            new ScannerThread(this).start();

            System.out.println("Warte auf eingehende Verbindung..");
            while (!this.stopping) {
                // accept a client
                Socket socket = this.serverSocket.accept();

                // create and start a thread for a client
                SocketThread socketThread = new SocketThread(socket);
                socketThread.start();
                try {
                    // wait for the clientThread to stop
                    socketThread.join();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }

            this.stop();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * Stop the server
     */
    public void stop() {
        try {
            this.serverSocket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
        System.out.println("Stoppe Server.");
        System.exit(0);
    }

    public static void main(String[] args) {
        new WetterServer(1234);
    }

    /**
     * Change value of boolean stopping
     * @param b new value for stopping
     */
    public void setStopping(boolean b) {
        this.stopping = b;
    }
}
