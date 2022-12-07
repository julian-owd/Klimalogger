<?php
// check if all of the parameters are set
if (isset($_GET["temp"]) && isset($_GET["humidity"]) && isset($_GET["co2"]) && isset($_GET["accessKey"])) {
    // data
    $temp = $_GET["temp"];
    $humidity = $_GET["humidity"];
    $co2 = $_GET["co2"];
	$accessKey = $_GET["accessKey"];
	
	// prevent forbidden access
	if (!checkAccess()) {
		header("HTTP/1.0 403 Forbidden");
		return;
	}
	
	// database connection
    include_once 'db_config.php';
	
	// prepare statement (prevents sql injection)
	$statement = $db->prepare("INSERT INTO `daten` (`temp`, `humidity`, `co2`) VALUES (?, ?, ?)");
	$statement->bind_param('ddd', $temp, $humidity, $co2);
	
	// execute statement
	if ($statement->execute()) {
		echo "New record created successfully";
	} else {
		echo "Execute failed: (" .$statement->errno . ") " . $statement->error;
	}
	
	// close statement
	$statement->close();
} else {
    header("HTTP/1.0 404 Not Found");
}

// checks for access key
function checkAccess() {
	return @$_GET['accessKey']=='m2nZGW1S';  
}
?>