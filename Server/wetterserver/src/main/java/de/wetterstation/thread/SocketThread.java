package de.wetterstation.thread;

import java.io.IOException;

import de.wetterstation.util.Socket;

public class SocketThread extends Thread {

    private Socket socket;

    public SocketThread(Socket socket) {
        this.socket = socket;
    }

    @Override
    public void run() {
        try {
            System.out.println("Verbindung zur Wetterstation hergestellt!");

            String line = "";
            while (true) {
                line = socket.readLine();
                if (line.equals("exit")) {
                    break;
                }
                double temperature = Double.parseDouble(line);
                System.out.println("Lese " + temperature);
                // Werte in DB eintragen
            }

            socket.close();
            System.out.println("Verbindung zur Wetterstation getrennt!");
            System.out.println("Versuche Verbindung wieder herzustellen!");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
