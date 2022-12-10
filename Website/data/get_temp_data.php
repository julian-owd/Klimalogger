<?php
header('Content-Type: application/json');

include_once "../db_config.php";

$query = "SELECT DATE_FORMAT(`datum`,'%Y,%m,%d,%H,%i,%S') AS datum, `temp` FROM (SELECT * FROM `daten` ORDER BY `id` DESC LIMIT 50) AS tmp ORDER BY `id` ASC;";
$result = $db->query($query);

$rows = array();
$table = array();

$table['cols'] = array( 
	array('label' => 'Uhrzeit', 'type' => 'datetime'),
	array('label' => 'Temperatur', 'type' => 'number')
);

while ($row = mysqli_fetch_array($result)) {
	$sub_array = array();
	$datetime = explode(",", $row["datum"]);
	$sub_array[] = array(
			"v" => 'Date(' . $datetime[0] . ',' . $datetime[1] - 1 . ',' . $datetime[2] . ',' . $datetime[3] . ',' . $datetime[4] . ',' . $datetime[5] . ',' . ')'
		);
	$sub_array[] = array(
			"v" => $row["temp"]
		);
		
	$rows[] = array(
			"c" => $sub_array
		);
}

$table['rows'] = $rows;

$result->close();
$db->close();

print json_encode($table);
?>
