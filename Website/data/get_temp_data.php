<?php
header('Content-Type: application/json');

if (isset($_GET['r'])) {
	
	$rId = $_GET['r'];
	include_once "../db_config.php";
	
	$statement = $db->prepare("SELECT DATE_FORMAT(`created`,'%Y,%m,%d,%H,%i,%S') AS datum, `temperature` FROM (SELECT * FROM `sensor` ORDER BY `id` DESC LIMIT 50) as tmp WHERE `rId`=? ORDER BY `id` ASC;");
	$statement->bind_param('i', $rId);
	$statement->execute();
	$statement->store_result();
	$statement->bind_result($date, $temperature);
	
	$rows = array();
	$table = array();
	
	$table['cols'] = array( 
		array('label' => 'Uhrzeit', 'type' => 'datetime'),
		array('label' => 'Temperatur', 'type' => 'number')
	);
	
	while ($statement->fetch()) {
		$sub_array = array();
		$datetime = explode(",", $date);
		$sub_array[] = array(
				"v" => 'Date(' . $datetime[0] . ',' . ($datetime[1] - 1) . ',' . $datetime[2] . ',' . $datetime[3] . ',' . $datetime[4] . ',' . $datetime[5] . ')'
			);
		$sub_array[] = array(
				"v" => number_format($temperature, 2)
			);
			
		$rows[] = array(
				"c" => $sub_array
			);
	}

	$table['rows'] = $rows;

	$statement->close();
	$db->close();

	print json_encode($table);
}
?>
