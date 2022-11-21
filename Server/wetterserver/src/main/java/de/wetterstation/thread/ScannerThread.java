package de.wetterstation.thread;

import java.util.Scanner;

import de.wetterstation.WetterServer;

public class ScannerThread extends Thread {

    private WetterServer wetterServer;

    public ScannerThread(WetterServer wetterServer) {
        this.wetterServer = wetterServer;
    }

    @Override
    public void run() {
        Scanner scanner = new Scanner(System.in);
        String line = "";
        while (!line.equals("exit")) {
            // read line from console until line equals "exit"
            line = scanner.nextLine();
        }
        // close scanner and initiate server stop
        scanner.close();
        this.wetterServer.setStopping(true);
        this.wetterServer.stop();
    }
}
