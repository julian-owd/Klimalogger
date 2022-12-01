<!DOCTYPE html>
<html>
    <head>
        <title>Klimalogger</title>
        <link rel="stylesheet" href="style.css">
    </head>
    <body>
        <h1>Hello World</h1>
        <h2>Hallo Welt</h2>
        
<?php
        include_once 'db_config.php';
        
        echo("connected to database<br />");

        $sql = "SELECT id, datum, temp, humidity, co2 FROM daten";
        foreach ($db->query($sql) as $row) {
            echo "<p>ID: ".$row['id']." Date: ".$row['datum']." Temp: ".$row['temp']." Humidity: ".$row['humidity']." CO2: ".$row['co2']."</p><br />";
        }
?>
    <a href="export_data.php">Export</a>
    </body>
</html>