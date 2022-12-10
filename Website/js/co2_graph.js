$(function() {
	google.charts.load('current', {'packages':['corechart']});
	google.charts.setOnLoadCallback(drawChart);
			
	function drawChart() {
		var jsonData = $.ajax({
			url: "data/get_co2_data.php",
			dataType: "json",
			async: false
		}).responseText;
				  
		// Create data table out of JSON data
		var data = new google.visualization.DataTable(jsonData);
		
		var dateFormatter = new google.visualization.DateFormat({pattern: 'dd.MM.yyyy HH:mm:ss'});
		dateFormatter.format(data, 0);
				
		var options = {
			title: 'Kohlenstoffdioxidgehalt in ppm',
			legend: {position: 'bottom'},
			chartArea: {width: '95%', height: '65%'},
			hAxis: {format: 'HH:mm:ss'},
			colors: ['#983b46']
		};
			
		var chart = new google.visualization.LineChart(document.getElementById('co2_chart'));
		chart.draw(data, options);
	}
});