<?php
// Database configuration
$host = "localhost";
$user = "root";
$password = "";
$database = "wetterstation"; 

// Create database connection
$db = new mysqli($host, $user, $password, $database);

// Check connection
if ($db->connect_error) {
    die("Verbindung zur Datenbank fehlgeschlagen: " . $db->connect_error);
}
