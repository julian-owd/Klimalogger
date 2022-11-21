package de.wetterstation.util;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

/**
 *  Ein-/Ausgabe �ber Sockets
 *  Ein Objekt der Klasse Socket kapselt eine Socket-Schnittstelle.
 *  Die Festlegung der Socket-Parameter kann nur bei der Objekt-Erzeugung
 *  vorgenommen werden.
 *  Ob die Verbindungsanforderung erfolgreich war, kann durch den R�ckgabewert
 *  der Methode connect() festgestellt werden
 *  Das Socket-Objekt gestattet im Zustand verbunden das Lesen bzw.
 *  Schreiben von  Strings von der bzw. auf die Socket-Schnittstelle.
 */
public class Socket {
  /** Der Hostname des Servers. */
  private String hostname;
  /** Der Port, auf dem der Server-Socket horcht. */
  private int port;
  /** Der Socket. */
  private java.net.Socket socket;
  /** Ein BufferdReader zum Lesen von Strings */
  private BufferedReader reader;

  /** Konstruktor
   *  Erzeugt einen neuen Socket mit der angegebenen Portnummer zu dem
   *  angegebenen Host.
   *  hostname der Hostname des Servers
   *  port der Port, auf dem der Server horcht
   */
  public Socket(String hostname, int port) throws IOException {
    this.hostname = hostname;
    this.port = port;
  }

  /** Konstruktor
   *  Erzeugt einen neuen Socket mit der angegebenen Socket.
   *  Wird vom ServerSocket benutzt.
   *  socket: der Socket, mit dem das Socket-Objekt arbeiten soll
   */
  public Socket(java.net.Socket socket) throws IOException {
    this.socket = socket;
    this.port = socket.getPort();
    this.hostname = socket.getRemoteSocketAddress().toString();
    reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
  }

  /** Schlie�t den Socket.
   *  Nach dem Schlie�en des Socket ist das Socket-Objekt
   *  nicht mehr verbunden.
   */
  public void close() throws IOException {
    reader.close();
    socket.close();
  }

  /** Verbindet das Socket-Objekt zu der entsprechenden
   *  Portnummer auf dem entsprechenden Host.
   *  Ging alles gut, ist das Socket-Objekt verbunden.
   *  return  true, wenn die Verbindung akzeptiert wurde
   */
  public boolean connect() {
    try {
      socket = new java.net.Socket(hostname, port);
      reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
    } catch (Exception e) {
      return false;
    }
    return true;
  }

//  --- Implementierung Leseoperationen ------------------------------

  /** Lesen eines Strings vom Socket. <
   *  Die Methode liest eine Zeile, die durch ein Zeilenendezeichen 
   *  ('\n' linefeed) abgeschlossen sein muss. Der zur�ckgegebene 
   *  String enth�lt die Zeile ohne Zeilenendezeichen.
   *  Die Methode blockiert, bis Eingabe-Daten verf�gbar sind.
   *  Im Fehlerfall liefert sie -1 oder erzeugt eine Ausnahme.
   *  return Der String oder null
   */
  public String readLine() throws IOException {
    return reader.readLine();
  }

  //  --- Implementierung Schreiboperationen ---------------------------

  /** Schreiben eines String auf den Socket.
   *  Diese Methode schreibt einen String zeichenweise auf den Socket.
   *  param s Der String
   */
  public void write(String s) throws IOException {
    socket.getOutputStream().write(s.getBytes());
  }
}
