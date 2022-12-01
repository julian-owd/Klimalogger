<?php
// example: http://localhost/wetter/insert_data.php?temp=23.2&humidity=54.3&co2=12.3
// tutorial: https://esp32io.com/tutorials/esp32-mysql
if (isset($_GET["temp"]) && isset($_GET["humidity"]) && isset($_GET["co2"])) {
    // data
    $temp = $_GET["temp"];
    $humidity = $_GET["humidity"];
    $co2 = $_GET["co2"];

    include_once 'db_config.php';

    // insert data
    $sql = "INSERT INTO daten (temp, humidity, co2) VALUES ($temp, $humidity, $co2)";

    if ($db->query($sql) === TRUE) {
        echo "New record created successfully";
    } else {
        echo "Error: " . $sql . " => " . $connection->error;
    }
} else {
    echo "all values must be set in HTTP request";
}
?>