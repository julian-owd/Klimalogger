<?php
include_once "db_config.php";

$query = "SELECT DATE_FORMAT(datum,'%H:%i:%S') AS datum, temp, humidity, co2 FROM (SELECT * FROM daten ORDER BY id DESC LIMIT 50) AS tmp ORDER BY id ASC";
$result = $db->query($query);

$data = array();
foreach ($result as $row) {
  $data[] = $row;
}

$result->close();
$db->close();

print json_encode($data);
?>
