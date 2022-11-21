package de.wetterstation;

import java.io.IOException;

import de.wetterstation.thread.ScannerThread;
import de.wetterstation.thread.SocketThread;
import de.wetterstation.util.ServerSocket;
import de.wetterstation.util.Socket;

public class WetterServer {

    private ServerSocket serverSocket;
    private boolean stopping = false;

    public WetterServer(int port) {
        try {
            System.out.println("Wetterstation Server\n");
            this.serverSocket = new ServerSocket(port);
            System.out.println("Starte Server..");

            new ScannerThread(this).start();

            System.out.println("Warte auf eingehende Verbindung..");
            while (!this.stopping) {
                Socket socket = this.serverSocket.accept();

                SocketThread socketThread = new SocketThread(socket);
                socketThread.start();
                try {
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

    public void setStopping(boolean b) {
        this.stopping = b;
    }
}
