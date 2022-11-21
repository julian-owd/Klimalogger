package de.wetterstation.util;

import java.io.IOException;

/**
 *  Ein-/Ausgabe �ber Sockets
 *  Ein Objekt der Klasse ServerSocket kapselt einen Server-Socket.
 *  Die Festlegung der ServerSocket-Parameter kann nur bei der Objekt-Erzeugung
 *  vorgenommen werden
 *  Das ServerSocket-Objekt kann Verbindungsanforderungen von Socket-Objekten
 *  entgegennehmen
 *  Ob die Verbindungsanforderung erfolgreich war, kann durch den R�ckgabewert
 *  der Methode  accept() festgestellt werden.  Diese liefert ein Socket-Objekt
 *  (Arbeitssocket), wenn  die Verbindung akzeptiert wurde
 *  Das ServerSocket-Objekt erm�glicht nach seiner Erzeugung nur das
 *  Entgegennehmen von Verbindungsanforderungen
 */
public class ServerSocket {
    /** Der Port, auf dem der ServerSocket horcht. */
  @SuppressWarnings("unused")
  private int localPort;
  /** Der ServerSocket */
  private java.net.ServerSocket serverSocket;

  /** Konstruktor
   *  Erzeugt einen neuen ServerSocket mit der angegebenen Portnummer.
   *  localPort der Port, auf dem der Server horcht
   */
  public ServerSocket(int localPort) throws IOException {
    this.localPort = localPort;
    serverSocket = new java.net.ServerSocket(localPort);
  }
  /** Warten auf eine Verbindungsanforderung.
   *  Der Server-Socket wartet in der Methode (blockierend) auf 
   *  eingehende Client-Verbindungen.
   *  return  Socket wenn die Verbindung akzeptiert wurde, sonst null
   */
  public Socket accept() throws IOException {
    return new Socket(serverSocket.accept());
  }
  /** Schlie�en des ServerSockets.
   *  Nach dem Schlie�en des ServerSocket kann der Server nicht mehr auf
   *  Verbindungsanforderungen reagieren.
   */
  public void close() throws IOException {
    serverSocket.close();
  }
}
