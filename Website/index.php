<!DOCTYPE html>
<html>
    <head>
        <title>Klimalogger</title>
        <link rel="stylesheet" href="css/style.css">
    </head>
    <body>
        <h1>Hello World</h1>
        <h2>Hallo Welt</h2>

        <!-- Änderung der Farbe der Graphen sinnvolL!! Wenn auch nur leicht -->
        <div class="temp-chart-container">
          <canvas id="tempCanvas"></canvas>
        </div>
        <div class="humidity-chart-container">
          <canvas id="humidityCanvas"></canvas>
        </div>
        <div class="co2-chart-container">
          <canvas id="co2Canvas"></canvas>
        </div>

        <script type="text/javascript" src="js/jquery.min.js"></script>
        <script type="text/javascript" src="js/chart.min.js"></script>
        <script type="text/javascript" src="js/graph.js"></script>


<?php
        include_once 'db_config.php';

        echo("connected to database<br />");

        $sql = "SELECT * FROM daten ORDER BY id DESC LIMIT 50;";
        foreach ($db->query($sql) as $row) {
            echo "<p>ID: ".$row['id']." Date: ".$row['datum']." Temp: ".$row['temp']." Humidity: ".$row['humidity']." CO2: ".$row['co2']."</p><br />";
        }
?>
    <a href="export_data.php">Export</a>
    </body>
</html>
