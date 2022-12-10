<?php
// example: http://localhost/wetter/data/insert_data.php?r=1&t=21.2&h=53.2&c=999&accessKey=m2nZGW1S
// check if all of the parameters are set
if (isset($_GET["r"]) && isset($_GET["t"]) && isset($_GET["h"]) && isset($_GET["c"]) && isset($_GET["accessKey"])) {
    // data
	$room = $_GET["r"];
    $temp = $_GET["t"];
    $humidity = $_GET["h"];
    $co2 = $_GET["c"];
	$accessKey = $_GET["accessKey"];
	
	// prevent forbidden access
	if (!checkAccess()) {
		header("HTTP/1.0 403 Forbidden");
		return;
	}
	
	// database connection
    include_once '../db_config.php';
	
	// prepare statement (prevents sql injection)
	$statement = $db->prepare("INSERT INTO `sensor` (`temperature`, `humidity`, `co2`, `rId`) VALUES (?, ?, ?, ?)");
	$statement->bind_param('dddd', $temp, $humidity, $co2, $room);
	
	// execute statement
	if ($statement->execute()) {
		echo "New record created successfully";
	} else {
		echo "Execute failed: (" .$statement->errno . ") " . $statement->error;
	}
	
	// close statement
	$statement->close();
	$db->close();
} else {
    header("HTTP/1.0 404 Not Found");
}

// checks for access key
function checkAccess() {
	return @$_GET['accessKey']=='m2nZGW1S';  
}
?>