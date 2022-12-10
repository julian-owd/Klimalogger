<!DOCTYPE html>
<html>
    <head>
        <title>Klimalogger</title>
		
		<link rel="icon" type="image/png" href="img/favicon.png"/>
		
		<link rel="stylesheet" href="css/style.css">
		<link rel="preconnect" href="https://fonts.googleapis.com">
		<link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
		<link href="https://fonts.googleapis.com/css2?family=Open+Sans:wght@300&display=swap" rel="stylesheet">
				
		<script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
		<script type="text/javascript" src="//ajax.googleapis.com/ajax/libs/jquery/1.10.2/jquery.min.js"></script>
		
		<script type="text/javascript" src="js/temp_graph.js"></script>
		<script type="text/javascript" src="js/humidity_graph.js"></script>
		<script type="text/javascript" src="js/co2_graph.js"></script>
		
    </head>
    <body>
		<div class="header">
			<h1>Klimalogger</h1>
			<div class="header-right">
				<a href="export_data.php">Export</a>
				<a href="http://wp.tls-gi.de/" target="_blank">TLS</a>
			</div>
		</div>
		
		<div class="current-data">
			
		
<?php
        include_once 'db_config.php';

        $sql = "SELECT DATE_FORMAT(`datum`,'%d.%m.%Y %H:%i:%S') AS datum, `temp`, `humidity`, `co2` FROM `daten` WHERE `id` = (SELECT MAX(`id`) FROM `daten`);";
        foreach ($db->query($sql) as $row) {
            echo "<h2>Aktuelle Temperatur: ".$row['temp']."° C</h2><br /><h2>Aktuelle Luftfeuchtigkeit: " .$row["humidity"] . "%</h2><br /><h2>Aktueller Kohlenstoffdioxidgehalt: ".$row['co2'] . "ppm</h2><h3>Letzte Aktualisierung: " . $row["datum"] . "</h3><br />";
        }
?>

		</div>
				
		<div id="temp_chart" style="width: 100%; height: 500px;"></div>
		<div id="humidity_chart" style="width: 100%; height: 500px;"></div>
		<div id="co2_chart" style="width: 100%; height: 500px;"></div>
		
    </body>
</html>
