<?php
// Database configuration
$host = "localhost";
$user = "root";
$password = "";
$database = "klimalogger"; 

// error reporting
mysqli_report(MYSQLI_REPORT_ERROR | MYSQLI_REPORT_STRICT);

// Create database connection
$db = new mysqli($host, $user, $password, $database);

// Check connection
if ($db->connect_error) {
    die("Verbindung zur Datenbank fehlgeschlagen: " . $db->connect_error);
}
?>