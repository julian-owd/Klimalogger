<!DOCTYPE html>
<html>
    <head>
        <title>Wetterstation</title>
        <link rel="stylesheet" href="style.css">
    </head>
    <body>
        <h1>Hello World</h1>
        <h2>Hallo Welt</h2>
        
<?php
        
        $config = parse_ini_file('config.ini', true);
        $con = new PDO('mysql:host='.$config['host'].';dbname='.$config['database'], $config['username'], $config['password']);

        if (!$con) {
            exit("Verbindungsfehler ".mysqli_connect_error());
        }
        echo("connected to database<br />");

        $sql = "SELECT id, datum, temp, humidity, co2 FROM daten";
        foreach ($con->query($sql) as $row) {
            echo "<p>ID: ".$row['id']." Date: ".$row['datum']." Temp: ".$row['temp']." Humidity: ".$row['humidity']." CO2: ".$row['co2']."</p><br />";
        }
?>
    </body>
</html>