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
				<select name="selectSection" id="selectSection" onchange="selectRoom()">
					<?php
						$rId = 1;
						if (isset($_GET["r"])) {
							$rId = $_GET["r"];
						}
						include_once 'db_config.php';
						$sql = "SELECT * FROM `room`;";
						foreach ($db->query($sql) as $row) {
							if ($row['id'] == $rId) {
								echo "<option value=". $row['id'] . " selected=selected>" . $row['label'] . "</option>";
							} else {
								echo "<option value=". $row['id'] . ">" . $row['label'] . "</option>";
							}
						}
					?>
				</select>
				
				<script>
					function selectRoom() {
						var selectBox = document.getElementById("selectSection");
						var rId = selectBox.options[selectBox.selectedIndex].value;
						
						
						if (!rId) {
							window.location.href = ("index.php");
						} else {
							window.location.href = ("index.php?r=" + rId);
						}
					}
				</script>
				
				<a href="data/export_data.php">Export</a>
				<a href="http://wp.tls-gi.de/" target="_blank">TLS</a>
			</div>
		</div>
		
		
			
<?php
		include_once 'db_config.php';
		
		$rId = 1;
		
		if (isset($_GET["r"])) {
			$rId = $_GET["r"];
		}
		
		$statement = $db->prepare("SELECT `label` FROM `room` WHERE `id`=?");
		$statement->bind_param('i', $rId);
		$statement->execute();
		$statement->store_result();
		if ($statement->num_rows == 0) {
			$rId = 0;
		}
			
		$statement->close();
					
		if (empty($rId)) {
			echo "<div class=error><h1>Der Raum konnte nicht gefunden werden!</h1><br />";
			echo "<a href=index.php>Zurück</a></div>";
			exit();
		}
		
		$statement = $db->prepare("SELECT DATE_FORMAT(`created`,'%d.%m.%Y %H:%i:%S') AS datum, `temperature`, `humidity`, `co2` FROM `sensor` WHERE `id`=(SELECT MAX(`id`) FROM `sensor` WHERE `rId`=?);");
		$statement->bind_param('i', $rId);
		$statement->execute();
		$statement->store_result();
		$statement->bind_result($date, $temperature, $humidity, $co2);
		
		if ($statement->num_rows == 0) {
			echo "<div class=error><h1>Es sind keine Daten für diesen Raum vorhanden!</h1><br />";
			echo "<a href=index.php>Zurück</a></div>";
			exit();
		}
		
		if ($statement->fetch()) {
			echo "<div class=current-data><h2>Aktuelle Temperatur: " . $temperature . "° C</h2><br /><h2>Aktuelle Luftfeuchtigkeit: " . $humidity . "%</h2><br /><h2>Aktueller Kohlenstoffdioxidgehalt: " . $co2 . "ppm</h2><h3>Letzte Aktualisierung: " . $date . "</h3><br /></div>";
		}
		
		$statement->close();
		$db->close();
?>
				
		<div id="temp_chart" style="width: 100%; height: 500px;"></div>
		<div id="humidity_chart" style="width: 100%; height: 500px;"></div>
		<div id="co2_chart" style="width: 100%; height: 500px;"></div>
		
    </body>
</html>
