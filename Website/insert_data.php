<?php
// example: http://localhost/wetter/insert_data.php?temp=23.2&humidity=54.3&co2=12.3
// tutorial: https://esp32io.com/tutorials/esp32-mysql
if (isset($_GET["temp"]) && isset($_GET["humidity"]) && isset($_GET["co2"])) {
    // data
    $temp = $_GET["temp"];
    $humidity = $_GET["humidity"];
    $co2 = $_GET["co2"];

    // login data for database
    $servername = "localhost";
    $username = "root";
    $password = "";
    $database = "wetterstation";

    // establish connection to database
    $connection = new mysqli($servername, $username, $password, $database);

    // check connection
    if ($connection->connect_error) {
      die("MySQL connection failed: " . $connection->connect_error);
    }

    // insert data
    $sql = "INSERT INTO daten (temp, humidity, co2) VALUES ($temp, $humidity, $co2)";

    if ($connection->query($sql) === TRUE) {
        echo "New record created successfully";
    } else {
        echo "Error: " . $sql . " => " . $connection->error;
    }
    $connection->close();
} else {
    echo "all values must be set in HTTP request";
}
?>