<?php 
 
include_once '../db_config.php';

// Fetch records from database 
$query = $db->query("SELECT `label`, `created`, `temperature`, `humidity`, `co2` FROM `sensor` JOIN `room` ON room.id = sensor.rId ORDER BY `rId` ASC, `created` ASC;");
  
if ($query->num_rows > 0) {
    $delimiter = ",";
    $filename = "klima-daten-".date('Y-m-d') . ".csv"; 
      
    // Create a file pointer 
    $f = fopen('php://memory', 'w'); 
      
    // Set column headers 
    $fields = array('ROOM', 'CREATED', 'TEMPERATURE', 'HUMIDITY', 'CO2');
    fputcsv($f, $fields, $delimiter); 
     
    // Output each row of the data, format line as csv and write to file pointer 
    while ($row = $query->fetch_assoc()) { 
        $lineData = array($row['label'], $row['created'], $row['temperature'], $row['humidity'], $row['co2']); 
        fputcsv($f, $lineData, $delimiter); 
    } 
    
    // Move back to beginning of file 
    fseek($f, 0); 
     
    // Set headers to download file rather than displayed 
    header('Content-Type: text/csv'); 
    header('Content-Disposition: attachment; filename="' . $filename . '";'); 
      
    //output all remaining data on a file pointer 
    fpassthru($f); 
} 
$db->close();
exit; 
  
?>