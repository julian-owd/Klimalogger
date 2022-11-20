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
        
        $con = new PDO('mysql:host=localhost;dbname=wetterstation', 'root', '');

        if (!$con) {
            exit("Verbindungsfehler ".mysqli_connect_error());
        }
        echo("connected to database<br />");

        $sql = "SELECT id, datum, temp FROM daten";
        foreach ($con->query($sql) as $row) {
            echo "<p>ID: ".$row['id']." Date: ".$row['datum']." Temp: ".$row['temp']."</p><br />";
        }
?>
    </body>
</html>