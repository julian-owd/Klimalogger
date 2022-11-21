package de.wetterstation.thread;

import java.io.IOException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.SQLException;

import de.wetterstation.util.Socket;

public class SocketThread extends Thread {

    private Socket socket;
    private Connection connection;

    public SocketThread(Socket socket) {
        this.socket = socket;
        try {
            // connect to database
            this.connection = DriverManager.getConnection("jdbc:mysql://localhost:3306", "root", "");
            if (this.connection == null) {
                System.out.println("Es konnte keine Verbindung zur Datenbank hergestellt werden!");
                return;
            }
            System.out.println("Verbindung zur Datenbank hergestellt!");
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void run() {
        try {
            System.out.println("Verbindung zur Wetterstation hergestellt!");

            String line = "";
            while (true) {
                // read line from client
                line = socket.readLine();
                if (line.equals("exit")) {
                    break;
                }
                double temperature = Double.parseDouble(line);
                System.out.println("Lese " + temperature);

                try {
                    // write data into database
                    PreparedStatement preparedStatement = this.connection
                            .prepareStatement("INSERT INTO `daten` (`temp`) VALUES ('" + temperature + "')");
                    preparedStatement.execute();
                    preparedStatement.close();
                } catch (SQLException e) {
                    e.printStackTrace();
                }
            }

            try {
                // close connection to database
                this.connection.close();
                System.out.println("Verbindung zur Datenbank getrennt!");
            } catch (SQLException e) {
                e.printStackTrace();
            }
            // close connection to client
            socket.close();
            System.out.println("Verbindung zur Wetterstation getrennt!");
            System.out.println("Versuche Verbindung wieder herzustellen!");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
